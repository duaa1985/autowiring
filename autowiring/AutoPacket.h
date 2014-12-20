// Copyright (C) 2012-2014 Leap Motion, Inc. All rights reserved.
#pragma once
#include "AnySharedPointer.h"
#include "at_exit.h"
#include "auto_id.h"
#include "AutoCheckout.h"
#include "DecorationDisposition.h"
#include "demangle.h"
#include "hash_tuple.h"
#include "is_any.h"
#include "is_shared_ptr.h"
#include "MicroAutoFilter.h"
#include "ObjectPool.h"
#include "TeardownNotifier.h"
#include <list>
#include <sstream>
#include <typeinfo>
#include CHRONO_HEADER
#include MEMORY_HEADER
#include TYPE_INDEX_HEADER
#include STL_UNORDERED_MAP
#include EXCEPTION_PTR_HEADER

class AutoPacket;
class AutoPacketInternal;
class AutoPacketFactory;
class AutoPacketProfiler;
struct AutoFilterDescriptor;

/// <summary>
/// A decorator-style processing packet
/// </summary>
/// <remarks>
/// A processing packet may be decorated with additional types as desired by the user.
/// The pipeline packet is not a type of context; querying the packet for an element of
/// a particular type will look for an element of precisely that type, not an inherited
/// type or a related interface.
///
/// Consumers who wish to advertise a particular field under multiple types must do so
/// manually with the Advertise function.
/// </remarks>
class AutoPacket:
  public std::enable_shared_from_this<AutoPacket>,
  public TeardownNotifier
{
private:
  AutoPacket(const AutoPacket& rhs) = delete;
  AutoPacket(AutoPacket&&) = delete;

public:
  // Must hold the lock to 'factory' when calling this constructor
  AutoPacket(AutoPacketFactory& factory, std::shared_ptr<void>&& outstanding);
  ~AutoPacket();

  struct Recipient {
    // The iterator pointing to the location where the satisfaction counter was inserted
    std::list<SatCounter>::iterator position;
  };

protected:
  // A pointer back to the factory that created us. Used for recording lifetime statistics.
  const std::shared_ptr<AutoPacketFactory> m_parentFactory;
  
  // The successor to this packet
  std::shared_ptr<AutoPacketInternal> m_successor;

  // Hold the time point at which this packet was last initalized.
  std::chrono::high_resolution_clock::time_point m_initTime;

  // Outstanding count local and remote holds:
  const std::shared_ptr<void> m_outstanding;

  // Saturation counters, constructed when the packet is created and reset each time thereafter
  std::list<SatCounter> m_satCounters;

  // The set of decorations currently attached to this object, and the associated lock:
  // Decorations are indexed first by type and second by pipe terminating type, if any.
  // NOTE: This is a disambiguation of function reference assignment, and avoids use of constexp.
  typedef std::unordered_map<std::type_index, DecorationDisposition> t_decorationMap;
  t_decorationMap m_decorations;
  mutable std::mutex m_lock;

  /// <summary>
  /// Checks out the decoration named by the specified type information and attaches the specified immediate pointer to it
  /// </summary>
  /// <remarks>
  /// An immediate checkout differs from a standard checkout in that the internally held decoration is only temporarily
  /// available.  Thus, callers are either satisfied at the point of decoration, or will not be satisfied for that
  /// type.
  /// </remarks>
  DecorationDisposition& CheckoutImmediateUnsafe(const std::type_info& ti, const void* pvImmed);

  /// <summary>
  /// Adds all AutoFilter argument information for a recipient
  /// </summary>
  void AddSatCounter(SatCounter& satCounter);

  /// <summary>
  /// Removes all AutoFilter argument information for a recipient
  /// </summary>
  void RemoveSatCounter(const SatCounter& satCounter);

  /// <summary>
  /// Marks the specified entry as being unsatisfiable
  /// </summary>
  void MarkUnsatisfiable(const std::type_info& info);

  /// <summary>
  /// Updates subscriber statuses given that the specified type information has been satisfied
  /// </summary>
  /// <param name="info">The decoration which was just added to this packet</param>
  /// <remarks>
  /// This method results in a call to the AutoFilter method on any subscribers which are
  /// satisfied by this decoration.
  /// </remarks>
  void UpdateSatisfaction(const std::type_info& info);

  /// <summary>
  /// Performs a "satisfaction pulse", which will avoid notifying any deferred filters
  /// </summary>
  /// <remarks>
  /// A satisfaction pulse will call any AutoFilter instances which are satisfied by the
  /// decoration of the passed decoration types.  Such filters will be called even if
  /// some optional inputs remain outstanding.
  /// </remarks>
  void PulseSatisfaction(DecorationDisposition* pTypeSubs[], size_t nInfos);

  /// <summary>Un-templated & locked component of Has</summary>
  bool UnsafeHas(const std::type_info& data) const;

  /// <summary>Un-templated & locked component of Checkout</summary>
  void UnsafeCheckout(AnySharedPointer* ptr, const std::type_info& data);

  /// <summary>Un-templated & locked component of CompleteCheckout</summary>
  void UnsafeComplete(bool ready, const std::type_info& ti, DecorationDisposition*& entry);

  /// <summary>
  /// Invoked from a checkout when a checkout has completed
  /// <param name="ready">Ready flag, set to false if the decoration should be marked unsatisfiable</param>
  void CompleteCheckout(bool ready, const std::type_info& data);

  /// <summary>
  /// Retrieves the decoration disposition corresponding to some type
  /// </summary>
  /// <returns>The disposition, if the decoration exists and is satisfied, otherwise nullptr</returns>
  const DecorationDisposition* GetDisposition(const std::type_info& ti) const;

  /// <summary>
  /// Retrieves the decoration disposition corresponding to some type
  /// </summary>
  /// <returns>The disposition, if the decoration exists and is satisfied, otherwise nullptr</returns>
  /// <remarks>
  /// This method also does type aliasing in order to avoid attempted references to undefined types
  /// </remarks>
  template<class T>
  const DecorationDisposition* GetDisposition(void) const {
    return GetDisposition(typeid(auto_id<T>));
  }

  /// <returns>True if the indicated type has been requested for use by some consumer</returns>
  bool HasSubscribers(const std::type_info& ti) const;

  /// <returns>A reference to the satisfaction counter for the specified type</returns>
  /// <remarks>
  /// If the type is not a subscriber GetSatisfaction().GetType() == nullptr will be true
  /// </remarks>
  const SatCounter& GetSatisfaction(const std::type_info& subscriber) const;

  /// <returns>All subscribers to the specified data</returns>
  std::list<SatCounter> GetSubscribers(const std::type_info& data) const;

  /// <returns>All decoration dispositions associated with the data type</returns>
  /// <remarks>
  /// This method is useful for determining whether flow conditions (broadcast, pipes
  /// immediate decorations) resulted in missed data.
  /// </remarks>
  std::list<DecorationDisposition> GetDispositions(const std::type_info& ti) const;

public:
  /// <returns>
  /// True if this packet posesses a decoration of the specified type
  /// </returns>
  /// <remarks>
  /// Although "AutoPacket &" and "const AutoPacket&" argument types will be
  /// satisfied, the AutoPacket does not "have" these types.
  /// </remarks>
  template<class T>
  bool Has(void) const {
    std::lock_guard<std::mutex> lk(m_lock);
    return UnsafeHas(typeid(auto_id<T>));
  }

  /// <summary>
  /// Detects the desired type, or throws an exception if such a type cannot be found
  /// </summary>
  template<class T>
  const T& Get(void) const {
    static_assert(!std::is_same<T, AnySharedPointer>::value, "Oops!");

    const T* retVal;
    if(!Get(retVal)) {
      std::stringstream ss;
      ss << "Attempted to obtain a type " << autowiring::demangle(retVal)
         << " which was not decorated on this packet";
      throw std::runtime_error(ss.str());
    }
    return *retVal;
  }

  /// <summary>
  /// Determines whether this pipeline packet contains an entry of the specified type
  /// </summary>
  /// <remarks>
  /// This method is also used by DecorateImmediate to extract pointers to data that is
  /// valid ONLY during recursive satisfaction calls.
  /// </remarks>
  template<class T>
  bool Get(const T*& out) const {
    const DecorationDisposition* pDisposition = GetDisposition<T>();
    if (pDisposition) {
      if (pDisposition->m_decoration) {
        out = static_cast<const T*>(pDisposition->m_decoration->ptr());
        return true;
      }

      // Second-chance satisfaction with an immediate
      if (pDisposition->m_pImmediate) {
        out = (T*) pDisposition->m_pImmediate;
        return true;
      }
    }

    out = nullptr;
    return false;
  }

  template<class T>
  bool DEPRECATED(Get(const std::shared_ptr<T>*& out) const, "This version of Get is deprecated due to the dangers it implies, do not use");

  /// <summary>
  /// Shared pointer specialization of const T*&, used to obtain the underlying shared pointer for some type T
  /// </summary>
  /// <remarks>
  /// This specialization cannot be used to obtain a decoration which has been attached to this packet via
  /// DecorateImmediate.
  /// </remarks>
  template<class T>
  bool Get(const std::shared_ptr<const T>*& out) const {
    // Decoration must be present and the shared pointer itself must also be present
    const DecorationDisposition* pDisposition = GetDisposition<T>();
    if (!pDisposition || !pDisposition->m_decoration) {
      out = nullptr;
      return false;
    }

    out = &pDisposition->m_decoration->as_unsafe<const T>();
    return true;
  }

  /// <summary>
  /// Shared pointer specialization, used to obtain the underlying shared pointer for some type T
  /// </summary>
  /// <remarks>
  /// This method can return an argument of DecorateImmediate as a shared_ptr<T> without a deleter.
  /// PROBLEM: This use case implies that holding shared_ptr references to decorations is NOT SAFE.
  /// </remarks>
  template<class T>
  bool Get(std::shared_ptr<const T>& out) const {
    std::lock_guard<std::mutex> lk(m_lock);
    auto deco = m_decorations.find(typeid(auto_id<T>));
    if(deco != m_decorations.end() && deco->second.satisfied) {
      auto& disposition = deco->second;
      if(disposition.m_decoration) {
        out = disposition.m_decoration->as_unsafe<T>();
        return true;
      }
    }
    out.reset();
    return false;
  }

  template<class T>
  const std::shared_ptr<const T>& GetShared(void) const {
    const std::shared_ptr<const T>* retVal;
    Get(retVal);
    return *retVal;
  }

  /// <summary>
  /// De-templated placement method
  /// </summary>
  void Put(const std::type_info& ti, SharedPointerSlot&& in);

  /// <summary>
  /// Transfers ownership of argument to AutoPacket
  /// </summary>
  /// <remarks>
  /// This method may throw an exception.  Ownership is unconditionally transferred to this class
  /// even in the event an exception is thrown, thus the passed pointer is guaranteed to be cleaned
  /// up properly in all cases.
  /// </remarks>
  template<class T>
  void Put(T* in) {
    Put(typeid(auto_id<T>), SharedPointerSlotT<T, false>(std::shared_ptr<T>(in)));
  }

  /// <summary>
  /// Shares ownership of argument with AutoPacket
  /// </summary>
  /// <remarks>
  /// This can be used to:
  /// - place data on the AutoPack from an ObjectPool
  /// - move data from one AutoPacket to another without copying
  /// - alias the type of a decoration on AutoPacket
  /// </remarks>
  template<class T>
  void Put(std::shared_ptr<T> in) {
    Put(typeid(auto_id<T>), SharedPointerSlotT<T, false>(std::move(in)));
  }

  /// <summary>Shares all broadcast data from this packet with the recipient packet</summary>
  /// <remarks>
  /// This method should ONLY be called during the final-call sequence.
  /// This method is expected to be used to bridge data to a sibling context.
  /// Therefore, only broadcast data will be shared, since pipes between sibling
  /// contexts cannot be defined.
  /// Furthermore, types that are unsatisfied in this context will not be marked as
  /// unsatisfied in the recipient - only present data will be provided.
  /// </remarks>
  void ForwardAll(std::shared_ptr<AutoPacket> recipient) const;

  /// <summary>
  /// Checks out the specified type, providing it to the caller to be filled in
  /// </summary>
  /// <param name="ptr">If set, the initial value that will be held by the checkout</param>
  /// <remarks>
  /// The caller must call Ready on the returned value before it falls out of scope in order
  /// to ensure that the checkout is eventually committed.  The checkout will be committed
  /// when it falls out of scope if so marked.
  /// </remarks>
  template<class T>
  AutoCheckout<T> Checkout(std::shared_ptr<T> ptr) {
    const std::type_info& data = typeid(auto_id<T>);

    /// Injunction to prevent existential loops:
    static_assert(!std::is_same<T, AutoPacket>::value, "Cannot decorate a packet with another packet");

    if(!ptr)
      throw std::runtime_error("Cannot checkout with shared_ptr == nullptr");

    // This allows us to install correct entries for decorated input requests
    AnySharedPointer any_ptr(ptr);
    {
      std::lock_guard<std::mutex> guard(m_lock);
      UnsafeCheckout(&any_ptr, data);
    }
    return AutoCheckout<T>(
      *this,
      ptr,
      &AutoPacket::CompleteCheckout
    );
  }

  template<class T>
  AutoCheckout<T> Checkout(void) {
    return Checkout(std::make_shared<T>());
  }

  /// <summary>
  /// Marks the named decoration as unsatisfiable
  /// </summary>
  /// <remarks>
  /// Marking a decoration as unsatisfiable immediately causes any filters with an optional
  /// input on this type to be called, if the remainder of their inputs are available.
  /// </remarks>
  template<class T>
  void Unsatisfiable(void) {
    {
      // Insert a null entry at this location:
      std::lock_guard<std::mutex> lk(m_lock);
      auto& entry = m_decorations[typeid(auto_id<T>)];
      entry.m_type = &typeid(auto_id<T>); // Ensure correct type if instantiated here
      if(entry.satisfied ||
         entry.isCheckedOut)
        throw std::runtime_error("Cannot mark a decoration as unsatisfiable when that decoration is already present on this packet");

      // Mark the entry as permanently checked-out
      entry.isCheckedOut = true;
    }

    // Now trigger a rescan:
    MarkUnsatisfiable(typeid(auto_id<T>));
  }

  /// <summary>
  /// Decorates this packet with a particular type
  /// </summary>
  /// <returns>A reference to the internally persisted object</returns>
  /// <remarks>
  /// The Decorate method is unconditional and will install the passed
  /// value regardless of whether any subscribers exist.
  /// </remarks>
  template<class T>
  const T& Decorate(T t) {
    return Decorate(std::make_shared<T>(std::forward<T>(t)));
  }

  /// <summary>
  /// Decoration method specialized for shared pointer types
  /// </summary>
  /// <remarks>
  /// This decoration method has the additional benefit that it will make direct use of the passed
  /// shared pointer.
  /// </remarks>
  template<class T>
  const T& Decorate(std::shared_ptr<T> t) {
    Checkout<T>(t).Ready();
    return *t;
  }

  /// <summary>
  /// Subscribers respond to the decoration arguments immediately or never for this packet.
  /// Optional argument resolution is forced for any subscriber requiring at least one
  /// argument of this method
  /// </summary>
  /// <remarks>
  /// Unlike Decorate, the arguments of DecorateImmediate are not copied.
  /// Each decoration is only valid for AutoFilters which are valid during this call.
  /// If multiple values are specified, all will be simultaneously made valid and
  /// then invalidated.
  /// </remarks>
  template<class T, class... Ts>
  void DecorateImmediate(const T& immed, const Ts&... immeds) {
    // None of the inputs may be shared pointers--if any of the inputs are shared pointers, they must be attached
    // to this packet via Decorate, or else dereferenced and used that way.
    static_assert(
      !is_any<is_shared_ptr<T>::value, is_shared_ptr<Ts>::value...>::value,
      "DecorateImmediate must not be used to attach a shared pointer, use Decorate on such a decoration instead"
    );

    // Perform standard decoration with a short initialization:
    std::unique_lock<std::mutex> lk(m_lock);
    DecorationDisposition* pTypeSubs[1 + sizeof...(Ts)] = {
      &CheckoutImmediateUnsafe(typeid(auto_id<T>), &immed),
      &CheckoutImmediateUnsafe(typeid(auto_id<Ts>), &immeds)...
    };
    lk.unlock();


    // Pulse satisfaction:
    MakeAtExit([this, &pTypeSubs] {
      // Mark entries as unsatisfiable:
      // IMPORTANT: isCheckedOut = true prevents subsequent decorations of this type
      for(DecorationDisposition*  pEntry : pTypeSubs) {
        pEntry->m_pImmediate = nullptr;
        pEntry->satisfied = false;
      }

      // Now trigger a rescan to hit any deferred, unsatisfiable entries:
#if autowiring_USE_LIBCXX
      for (const std::type_info* ti : {&typeid(auto_id<T>), &typeid(auto_id<Ts>)...})
        MarkUnsatisfiable(*ti);
#else
      bool dummy[] = {
        (MarkUnsatisfiable(typeid(auto_id<T>)), false),
        (MarkUnsatisfiable(typeid(auto_id<Ts>)), false)...
      };
      (void)dummy;
#endif
    }),
    PulseSatisfaction(pTypeSubs, 1 + sizeof...(Ts));
  }

  /// <summary>
  /// Adds a recipient for data associated only with this issuance of the packet.
  /// </summary>
  /// <remarks>
  /// This method is not idempotent.  The returned Recipient structure may be used to remove
  /// the recipient safely at any point.  The caller MUST NOT attempt 
  /// </remarks>
  Recipient AddRecipient(const AutoFilterDescriptor& descriptor);

  /// <summary>
  /// Removes a previously added packet recipient
  /// </summary>
  void RemoveRecipient(Recipient&& recipient);

  /// <summary>
  /// Convenience overload, identical in behavior to AddRecipient
  /// </summary>
  template<class Fx>
  Recipient operator+=(Fx&& fx) {
    return AddRecipient(AutoFilterDescriptor(std::forward<Fx&&>(fx)));
  }

  /// <summary>
  /// Convenience overload, provided to allow the attachment of receive-only filters to a const AutoPacket
  /// </summary>
  template<class Fx>
  const AutoPacket& operator+=(Fx&& fx) const;

  /// <returns>A reference to the satisfaction counter for the specified type</returns>
  /// <remarks>
  /// If the type is not a subscriber GetSatisfaction().GetType() == nullptr will be true
  /// </remarks>
  template<class T>
  inline const SatCounter& GetSatisfaction(void) const { return GetSatisfaction(typeid(auto_id<T>)); }

  /// <returns>All subscribers to the specified data</returns>
  template<class T>
  inline std::list<SatCounter> GetSubscribers(void) const {
    return GetSubscribers(typeid(auto_id<T>));
  }

  /// <returns>All decoration dispositions associated with the data type</returns>
  /// <remarks>
  /// This method is useful for determining whether flow conditions (broadcast, pipes
  /// immediate decorations) resulted in missed data.
  /// </remarks>
  template<class T>
  inline std::list<DecorationDisposition> GetDispositions(void) const {
    return GetDispositions(typeid(auto_id<T>));
  }

  /// <summary>
  /// Returns the next packet that will be issued by the packet factory in this context relative to this context
  /// </summary>
  std::shared_ptr<AutoPacket> Successor(void);
  
  /// <returns>True if the indicated type has been requested for use by some consumer</returns>
  template<class T>
  bool HasSubscribers(void) const {
    return HasSubscribers(typeid(auto_id<T>));
  }
};

#include "CallExtractor.h"

template<class Fx>
const AutoPacket& AutoPacket::operator+=(Fx&& fx) const
{
  static_assert(
    !CallExtractor<decltype(&Fx::operator())>::has_outputs,
    "Cannot add an AutoFilter to a const AutoPacket if any of its arguments are output types"
  );
  *const_cast<AutoPacket*>(this) += std::forward<Fx&&>(fx);
  return *this;
}

template<class T>
bool AutoPacket::Get(const std::shared_ptr<T>*& out) const {
  static_assert(!std::is_const<T>::value, "Overload resolution selected an incorrect version of Get");

  const DecorationDisposition* pDisposition = GetDisposition<T>();
  if (!pDisposition || !pDisposition->m_decoration) {
    out = nullptr;
    return false;
  }

  out = &pDisposition->m_decoration->as_unsafe<T>();
  return true;
}