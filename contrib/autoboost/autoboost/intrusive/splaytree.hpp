/////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga  2007-2014
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/intrusive for documentation.
//
/////////////////////////////////////////////////////////////////////////////
#ifndef AUTOBOOST_INTRUSIVE_SPLAYTREE_HPP
#define AUTOBOOST_INTRUSIVE_SPLAYTREE_HPP

#if defined(_MSC_VER)
#  pragma once
#endif

#include <autoboost/intrusive/detail/config_begin.hpp>
#include <autoboost/intrusive/intrusive_fwd.hpp>
#include <cstddef>
#include <functional>
#include <utility>

#include <autoboost/static_assert.hpp>
#include <autoboost/intrusive/bstree.hpp>
#include <autoboost/intrusive/detail/tree_node.hpp>
#include <autoboost/intrusive/detail/mpl.hpp>
#include <autoboost/intrusive/pointer_traits.hpp>
#include <autoboost/intrusive/detail/function_detector.hpp>
#include <autoboost/intrusive/detail/get_value_traits.hpp>
#include <autoboost/intrusive/splaytree_algorithms.hpp>
#include <autoboost/intrusive/link_mode.hpp>
#include <autoboost/intrusive/detail/key_nodeptr_comp.hpp>
#include <autoboost/move/utility_core.hpp>

namespace autoboost {
namespace intrusive {

/// @cond

struct splaytree_defaults
{
   typedef default_bstree_hook_applier proto_value_traits;
   static const bool constant_time_size = true;
   typedef std::size_t size_type;
   typedef void compare;
   typedef void header_holder_type;
};

/// @endcond

//! The class template splaytree is an intrusive splay tree container that
//! is used to construct intrusive splay_set and splay_multiset containers. The no-throw
//! guarantee holds only, if the value_compare object
//! doesn't throw.
//!
//! The template parameter \c T is the type to be managed by the container.
//! The user can specify additional options and if no options are provided
//! default options are used.
//!
//! The container supports the following options:
//! \c base_hook<>/member_hook<>/value_traits<>,
//! \c constant_time_size<>, \c size_type<> and
//! \c compare<>.
#if defined(AUTOBOOST_INTRUSIVE_DOXYGEN_INVOKED)
template<class T, class ...Options>
#else
template<class ValueTraits, class VoidOrKeyComp, class SizeType, bool ConstantTimeSize, typename HeaderHolder>
#endif
class splaytree_impl
   /// @cond
   :  public bstree_impl<ValueTraits, VoidOrKeyComp, SizeType, ConstantTimeSize, SplayTreeAlgorithms, HeaderHolder>
   /// @endcond
{
   public:
   typedef ValueTraits                                               value_traits;
   /// @cond
   typedef bstree_impl< ValueTraits, VoidOrKeyComp, SizeType
                      , ConstantTimeSize, SplayTreeAlgorithms
                      , HeaderHolder>                               tree_type;
   typedef tree_type                                                 implementation_defined;
   /// @endcond

   typedef typename implementation_defined::pointer                  pointer;
   typedef typename implementation_defined::const_pointer            const_pointer;
   typedef typename implementation_defined::value_type               value_type;
   typedef typename implementation_defined::key_type                 key_type;
   typedef typename implementation_defined::reference                reference;
   typedef typename implementation_defined::const_reference          const_reference;
   typedef typename implementation_defined::difference_type          difference_type;
   typedef typename implementation_defined::size_type                size_type;
   typedef typename implementation_defined::value_compare            value_compare;
   typedef typename implementation_defined::key_compare              key_compare;
   typedef typename implementation_defined::iterator                 iterator;
   typedef typename implementation_defined::const_iterator           const_iterator;
   typedef typename implementation_defined::reverse_iterator         reverse_iterator;
   typedef typename implementation_defined::const_reverse_iterator   const_reverse_iterator;
   typedef typename implementation_defined::node_traits              node_traits;
   typedef typename implementation_defined::node                     node;
   typedef typename implementation_defined::node_ptr                 node_ptr;
   typedef typename implementation_defined::const_node_ptr           const_node_ptr;
   typedef typename implementation_defined::node_algorithms          node_algorithms;

   static const bool constant_time_size = implementation_defined::constant_time_size;
   /// @cond
   private:

   //noncopyable
   AUTOBOOST_MOVABLE_BUT_NOT_COPYABLE(splaytree_impl)

   /// @endcond

   public:

   typedef typename implementation_defined::insert_commit_data insert_commit_data;

   //! @copydoc ::autoboost::intrusive::bstree::bstree(const value_compare &,const value_traits &)
   explicit splaytree_impl( const value_compare &cmp = value_compare()
                          , const value_traits &v_traits = value_traits())
      :  tree_type(cmp, v_traits)
   {}

   //! @copydoc ::autoboost::intrusive::bstree::bstree(bool,Iterator,Iterator,const value_compare &,const value_traits &)
   template<class Iterator>
   splaytree_impl( bool unique, Iterator b, Iterator e
              , const value_compare &cmp     = value_compare()
              , const value_traits &v_traits = value_traits())
      : tree_type(cmp, v_traits)
   {
      if(unique)
         this->insert_unique(b, e);
      else
         this->insert_equal(b, e);
   }

   //! @copydoc ::autoboost::intrusive::bstree::bstree(bstree &&)
   splaytree_impl(AUTOBOOST_RV_REF(splaytree_impl) x)
      :  tree_type(::autoboost::move(static_cast<tree_type&>(x)))
   {}

   //! @copydoc ::autoboost::intrusive::bstree::operator=(bstree &&)
   splaytree_impl& operator=(AUTOBOOST_RV_REF(splaytree_impl) x)
   {  return static_cast<splaytree_impl&>(tree_type::operator=(::autoboost::move(static_cast<tree_type&>(x)))); }

   #ifdef AUTOBOOST_INTRUSIVE_DOXYGEN_INVOKED
   //! @copydoc ::autoboost::intrusive::bstree::~bstree()
   ~splaytree_impl();

   //! @copydoc ::autoboost::intrusive::bstree::begin()
   iterator begin();

   //! @copydoc ::autoboost::intrusive::bstree::begin()const
   const_iterator begin() const;

   //! @copydoc ::autoboost::intrusive::bstree::cbegin()const
   const_iterator cbegin() const;

   //! @copydoc ::autoboost::intrusive::bstree::end()
   iterator end();

   //! @copydoc ::autoboost::intrusive::bstree::end()const
   const_iterator end() const;

   //! @copydoc ::autoboost::intrusive::bstree::cend()const
   const_iterator cend() const;

   //! @copydoc ::autoboost::intrusive::bstree::rbegin()
   reverse_iterator rbegin();

   //! @copydoc ::autoboost::intrusive::bstree::rbegin()const
   const_reverse_iterator rbegin() const;

   //! @copydoc ::autoboost::intrusive::bstree::crbegin()const
   const_reverse_iterator crbegin() const;

   //! @copydoc ::autoboost::intrusive::bstree::rend()
   reverse_iterator rend();

   //! @copydoc ::autoboost::intrusive::bstree::rend()const
   const_reverse_iterator rend() const;

   //! @copydoc ::autoboost::intrusive::bstree::crend()const
   const_reverse_iterator crend() const;

   //! @copydoc ::autoboost::intrusive::bstree::container_from_end_iterator(iterator)
   static splaytree_impl &container_from_end_iterator(iterator end_iterator);

   //! @copydoc ::autoboost::intrusive::bstree::container_from_end_iterator(const_iterator)
   static const splaytree_impl &container_from_end_iterator(const_iterator end_iterator);

   //! @copydoc ::autoboost::intrusive::bstree::container_from_iterator(iterator)
   static splaytree_impl &container_from_iterator(iterator it);

   //! @copydoc ::autoboost::intrusive::bstree::container_from_iterator(const_iterator)
   static const splaytree_impl &container_from_iterator(const_iterator it);

   //! @copydoc ::autoboost::intrusive::bstree::key_comp()const
   key_compare key_comp() const;

   //! @copydoc ::autoboost::intrusive::bstree::value_comp()const
   value_compare value_comp() const;

   //! @copydoc ::autoboost::intrusive::bstree::empty()const
   bool empty() const;

   //! @copydoc ::autoboost::intrusive::bstree::size()const
   size_type size() const;

   //! @copydoc ::autoboost::intrusive::bstree::swap
   void swap(splaytree_impl& other);

   //! @copydoc ::autoboost::intrusive::bstree::clone_from
   template <class Cloner, class Disposer>
   void clone_from(const splaytree_impl &src, Cloner cloner, Disposer disposer);

   //! @copydoc ::autoboost::intrusive::bstree::insert_equal(reference)
   iterator insert_equal(reference value);

   //! @copydoc ::autoboost::intrusive::bstree::insert_equal(const_iterator,reference)
   iterator insert_equal(const_iterator hint, reference value);

   //! @copydoc ::autoboost::intrusive::bstree::insert_equal(Iterator,Iterator)
   template<class Iterator>
   void insert_equal(Iterator b, Iterator e);

   //! @copydoc ::autoboost::intrusive::bstree::insert_unique(reference)
   std::pair<iterator, bool> insert_unique(reference value);

   //! @copydoc ::autoboost::intrusive::bstree::insert_unique(const_iterator,reference)
   iterator insert_unique(const_iterator hint, reference value);

   //! @copydoc ::autoboost::intrusive::bstree::insert_unique_check(const KeyType&,KeyValueCompare,insert_commit_data&)
   template<class KeyType, class KeyValueCompare>
   std::pair<iterator, bool> insert_unique_check
      (const KeyType &key, KeyValueCompare key_value_comp, insert_commit_data &commit_data);

   //! @copydoc ::autoboost::intrusive::bstree::insert_unique_check(const_iterator,const KeyType&,KeyValueCompare,insert_commit_data&)
   template<class KeyType, class KeyValueCompare>
   std::pair<iterator, bool> insert_unique_check
      (const_iterator hint, const KeyType &key
      ,KeyValueCompare key_value_comp, insert_commit_data &commit_data);

   //! @copydoc ::autoboost::intrusive::bstree::insert_unique_commit
   iterator insert_unique_commit(reference value, const insert_commit_data &commit_data);

   //! @copydoc ::autoboost::intrusive::bstree::insert_unique(Iterator,Iterator)
   template<class Iterator>
   void insert_unique(Iterator b, Iterator e);

   //! @copydoc ::autoboost::intrusive::bstree::insert_before
   iterator insert_before(const_iterator pos, reference value);

   //! @copydoc ::autoboost::intrusive::bstree::push_back
   void push_back(reference value);

   //! @copydoc ::autoboost::intrusive::bstree::push_front
   void push_front(reference value);

   //! @copydoc ::autoboost::intrusive::bstree::erase(const_iterator)
   iterator erase(const_iterator i);

   //! @copydoc ::autoboost::intrusive::bstree::erase(const_iterator,const_iterator)
   iterator erase(const_iterator b, const_iterator e);

   //! @copydoc ::autoboost::intrusive::bstree::erase(const_reference)
   size_type erase(const_reference value);

   //! @copydoc ::autoboost::intrusive::bstree::erase(const KeyType&,KeyValueCompare)
   template<class KeyType, class KeyValueCompare>
   size_type erase(const KeyType& key, KeyValueCompare comp);

   //! @copydoc ::autoboost::intrusive::bstree::erase_and_dispose(const_iterator,Disposer)
   template<class Disposer>
   iterator erase_and_dispose(const_iterator i, Disposer disposer);

   //! @copydoc ::autoboost::intrusive::bstree::erase_and_dispose(const_iterator,const_iterator,Disposer)
   template<class Disposer>
   iterator erase_and_dispose(const_iterator b, const_iterator e, Disposer disposer);

   //! @copydoc ::autoboost::intrusive::bstree::erase_and_dispose(const_reference, Disposer)
   template<class Disposer>
   size_type erase_and_dispose(const_reference value, Disposer disposer);

   //! @copydoc ::autoboost::intrusive::bstree::erase_and_dispose(const KeyType&,KeyValueCompare,Disposer)
   template<class KeyType, class KeyValueCompare, class Disposer>
   size_type erase_and_dispose(const KeyType& key, KeyValueCompare comp, Disposer disposer);

   //! @copydoc ::autoboost::intrusive::bstree::clear
   void clear();

   //! @copydoc ::autoboost::intrusive::bstree::clear_and_dispose
   template<class Disposer>
   void clear_and_dispose(Disposer disposer);

   //! @copydoc ::autoboost::intrusive::bstree::count(const_reference)const
   //! Additional note: non-const function, splaying is performed.
   size_type count(const_reference value);

   //! @copydoc ::autoboost::intrusive::bstree::count(const KeyType&,KeyValueCompare)const
   //! Additional note: non-const function, splaying is performed.
   template<class KeyType, class KeyValueCompare>
   size_type count(const KeyType &key, KeyValueCompare comp);

   //! @copydoc ::autoboost::intrusive::bstree::count(const_reference)const
   //! Additional note: const function, no splaying is performed
   size_type count(const_reference value) const;

   //! @copydoc ::autoboost::intrusive::bstree::count(const KeyType&,KeyValueCompare)const
   //! Additional note: const function, no splaying is performed
   template<class KeyType, class KeyValueCompare>
   size_type count(const KeyType &key, KeyValueCompare comp) const;

   //! @copydoc ::autoboost::intrusive::bstree::lower_bound(const_reference)
   //! Additional note: non-const function, splaying is performed.
   iterator lower_bound(const_reference value);

   //! @copydoc ::autoboost::intrusive::bstree::lower_bound(const_reference)const
   //! Additional note: const function, no splaying is performed
   const_iterator lower_bound(const_reference value) const;

   //! @copydoc ::autoboost::intrusive::bstree::lower_bound(const KeyType&,KeyValueCompare)
   //! Additional note: non-const function, splaying is performed for the first
   //! element of the equal range of "key"
   template<class KeyType, class KeyValueCompare>
   iterator lower_bound(const KeyType &key, KeyValueCompare comp);

   //! @copydoc ::autoboost::intrusive::bstree::lower_bound(const KeyType&,KeyValueCompare)const
   //! Additional note: const function, no splaying is performed
   template<class KeyType, class KeyValueCompare>
   const_iterator lower_bound(const KeyType &key, KeyValueCompare comp) const;

   //! @copydoc ::autoboost::intrusive::bstree::upper_bound(const_reference)
   //! Additional note: non-const function, splaying is performed for the first
   //! element of the equal range of "value"
   iterator upper_bound(const_reference value);

   //! @copydoc ::autoboost::intrusive::bstree::upper_bound(const_reference)const
   //! Additional note: const function, no splaying is performed
   const_iterator upper_bound(const_reference value) const;

   //! @copydoc ::autoboost::intrusive::bstree::upper_bound(const KeyType&,KeyValueCompare)
   //! Additional note: non-const function, splaying is performed for the first
   //! element of the equal range of "key"
   template<class KeyType, class KeyValueCompare>
   iterator upper_bound(const KeyType &key, KeyValueCompare comp);

   //! @copydoc ::autoboost::intrusive::bstree::upper_bound(const KeyType&,KeyValueCompare)const
   //! Additional note: const function, no splaying is performed
   template<class KeyType, class KeyValueCompare>
   const_iterator upper_bound(const KeyType &key, KeyValueCompare comp) const;

   //! @copydoc ::autoboost::intrusive::bstree::find(const_reference)
   //! Additional note: non-const function, splaying is performed for the first
   //! element of the equal range of "value"
   iterator find(const_reference value);

   //! @copydoc ::autoboost::intrusive::bstree::find(const_reference)const
   //! Additional note: const function, no splaying is performed
   const_iterator find(const_reference value) const;

   //! @copydoc ::autoboost::intrusive::bstree::find(const KeyType&,KeyValueCompare)
   //! Additional note: non-const function, splaying is performed for the first
   //! element of the equal range of "key"
   template<class KeyType, class KeyValueCompare>
   iterator find(const KeyType &key, KeyValueCompare comp);

   //! @copydoc ::autoboost::intrusive::bstree::find(const KeyType&,KeyValueCompare)const
   //! Additional note: const function, no splaying is performed
   template<class KeyType, class KeyValueCompare>
   const_iterator find(const KeyType &key, KeyValueCompare comp) const;

   //! @copydoc ::autoboost::intrusive::bstree::equal_range(const_reference)
   //! Additional note: non-const function, splaying is performed for the first
   //! element of the equal range of "value"
   std::pair<iterator, iterator> equal_range(const_reference value);

   //! @copydoc ::autoboost::intrusive::bstree::equal_range(const_reference)const
   //! Additional note: const function, no splaying is performed
   std::pair<const_iterator, const_iterator> equal_range(const_reference value) const;

   //! @copydoc ::autoboost::intrusive::bstree::equal_range(const KeyType&,KeyValueCompare)
   //! Additional note: non-const function, splaying is performed for the first
   //! element of the equal range of "key"
   template<class KeyType, class KeyValueCompare>
   std::pair<iterator, iterator> equal_range(const KeyType &key, KeyValueCompare comp);

   //! @copydoc ::autoboost::intrusive::bstree::equal_range(const KeyType&,KeyValueCompare)const
   //! Additional note: const function, no splaying is performed
   template<class KeyType, class KeyValueCompare>
   std::pair<const_iterator, const_iterator> equal_range(const KeyType &key, KeyValueCompare comp) const;

   //! @copydoc ::autoboost::intrusive::bstree::bounded_range(const_reference,const_reference,bool,bool)
   std::pair<iterator,iterator> bounded_range
      (const_reference lower_value, const_reference upper_value, bool left_closed, bool right_closed);

   //! @copydoc ::autoboost::intrusive::bstree::bounded_range(const KeyType&,const KeyType&,KeyValueCompare,bool,bool)
   template<class KeyType, class KeyValueCompare>
   std::pair<iterator,iterator> bounded_range
      (const KeyType& lower_key, const KeyType& upper_key, KeyValueCompare comp, bool left_closed, bool right_closed);

   //! @copydoc ::autoboost::intrusive::bstree::bounded_range(const_reference,const_reference,bool,bool)const
   std::pair<const_iterator, const_iterator> bounded_range
      (const_reference lower_value, const_reference upper_value, bool left_closed, bool right_closed) const;

   //! @copydoc ::autoboost::intrusive::bstree::bounded_range(const KeyType&,const KeyType&,KeyValueCompare,bool,bool)const
   template<class KeyType, class KeyValueCompare>
   std::pair<const_iterator, const_iterator> bounded_range
         (const KeyType& lower_key, const KeyType& upper_key, KeyValueCompare comp, bool left_closed, bool right_closed) const;

   //! @copydoc ::autoboost::intrusive::bstree::s_iterator_to(reference)
   static iterator s_iterator_to(reference value);

   //! @copydoc ::autoboost::intrusive::bstree::s_iterator_to(const_reference)
   static const_iterator s_iterator_to(const_reference value);

   //! @copydoc ::autoboost::intrusive::bstree::iterator_to(reference)
   iterator iterator_to(reference value);

   //! @copydoc ::autoboost::intrusive::bstree::iterator_to(const_reference)const
   const_iterator iterator_to(const_reference value) const;

   //! @copydoc ::autoboost::intrusive::bstree::init_node(reference)
   static void init_node(reference value);

   //! @copydoc ::autoboost::intrusive::bstree::unlink_leftmost_without_rebalance
   pointer unlink_leftmost_without_rebalance();

   //! @copydoc ::autoboost::intrusive::bstree::replace_node
   void replace_node(iterator replace_this, reference with_this);

   //! @copydoc ::autoboost::intrusive::bstree::remove_node
   void remove_node(reference value);

   #endif   //#ifdef AUTOBOOST_INTRUSIVE_DOXYGEN_INVOKED

   //! <b>Requires</b>: i must be a valid iterator of *this.
   //!
   //! <b>Effects</b>: Rearranges the container so that the element pointed by i
   //!   is placed as the root of the tree, improving future searches of this value.
   //!
   //! <b>Complexity</b>: Amortized logarithmic.
   //!
   //! <b>Throws</b>: Nothing.
   void splay_up(iterator i)
   {  return node_algorithms::splay_up(i.pointed_node(), tree_type::header_ptr());   }

   //! <b>Effects</b>: Rearranges the container so that if *this stores an element
   //!   with a key equivalent to value the element is placed as the root of the
   //!   tree. If the element is not present returns the last node compared with the key.
   //!   If the tree is empty, end() is returned.
   //!
   //! <b>Complexity</b>: Amortized logarithmic.
   //!
   //! <b>Returns</b>: An iterator to the new root of the tree, end() if the tree is empty.
   //!
   //! <b>Throws</b>: If the comparison functor throws.
   template<class KeyType, class KeyValueCompare>
   iterator splay_down(const KeyType &key, KeyValueCompare comp)
   {
      detail::key_nodeptr_comp<value_compare, value_traits>
         key_node_comp(comp, &this->get_value_traits());
      node_ptr r = node_algorithms::splay_down(tree_type::header_ptr(), key, key_node_comp);
      return iterator(r, this->priv_value_traits_ptr());
   }

   //! <b>Effects</b>: Rearranges the container so that if *this stores an element
   //!   with a key equivalent to value the element is placed as the root of the
   //!   tree.
   //!
   //! <b>Complexity</b>: Amortized logarithmic.
   //!
   //! <b>Returns</b>: An iterator to the new root of the tree, end() if the tree is empty.
   //!
   //! <b>Throws</b>: If the predicate throws.
   iterator splay_down(const_reference value)
   {  return this->splay_down(value, this->value_comp());   }

   #ifdef AUTOBOOST_INTRUSIVE_DOXYGEN_INVOKED
   //! @copydoc ::autoboost::intrusive::bstree::rebalance
   void rebalance();

   //! @copydoc ::autoboost::intrusive::bstree::rebalance_subtree
   iterator rebalance_subtree(iterator root);
   #endif   //#ifdef AUTOBOOST_INTRUSIVE_DOXYGEN_INVOKED
};

#if defined(AUTOBOOST_INTRUSIVE_DOXYGEN_INVOKED)

template<class T, class ...Options>
bool operator< (const splaytree_impl<T, Options...> &x, const splaytree_impl<T, Options...> &y);

template<class T, class ...Options>
bool operator==(const splaytree_impl<T, Options...> &x, const splaytree_impl<T, Options...> &y);

template<class T, class ...Options>
bool operator!= (const splaytree_impl<T, Options...> &x, const splaytree_impl<T, Options...> &y);

template<class T, class ...Options>
bool operator>(const splaytree_impl<T, Options...> &x, const splaytree_impl<T, Options...> &y);

template<class T, class ...Options>
bool operator<=(const splaytree_impl<T, Options...> &x, const splaytree_impl<T, Options...> &y);

template<class T, class ...Options>
bool operator>=(const splaytree_impl<T, Options...> &x, const splaytree_impl<T, Options...> &y);

template<class T, class ...Options>
void swap(splaytree_impl<T, Options...> &x, splaytree_impl<T, Options...> &y);

#endif   //#if defined(AUTOBOOST_INTRUSIVE_DOXYGEN_INVOKED)

//! Helper metafunction to define a \c splaytree that yields to the same type when the
//! same options (either explicitly or implicitly) are used.
#if defined(AUTOBOOST_INTRUSIVE_DOXYGEN_INVOKED) || defined(AUTOBOOST_INTRUSIVE_VARIADIC_TEMPLATES)
template<class T, class ...Options>
#else
template<class T, class O1 = void, class O2 = void
                , class O3 = void, class O4 = void
                , class O5 = void>
#endif
struct make_splaytree
{
   /// @cond
   typedef typename pack_options
      < splaytree_defaults,
      #if !defined(AUTOBOOST_INTRUSIVE_VARIADIC_TEMPLATES)
      O1, O2, O3, O4, O5
      #else
      Options...
      #endif
      >::type packed_options;

   typedef typename detail::get_value_traits
      <T, typename packed_options::proto_value_traits>::type value_traits;
   typedef typename detail::get_header_holder_type
      < value_traits, typename packed_options::header_holder_type >::type header_holder_type;

   typedef splaytree_impl
         < value_traits
         , typename packed_options::compare
         , typename packed_options::size_type
         , packed_options::constant_time_size
         , header_holder_type
         > implementation_defined;
   /// @endcond
   typedef implementation_defined type;
};


#ifndef AUTOBOOST_INTRUSIVE_DOXYGEN_INVOKED

#if !defined(AUTOBOOST_INTRUSIVE_VARIADIC_TEMPLATES)
template<class T, class O1, class O2, class O3, class O4, class O5>
#else
template<class T, class ...Options>
#endif
class splaytree
   :  public make_splaytree<T,
      #if !defined(AUTOBOOST_INTRUSIVE_VARIADIC_TEMPLATES)
      O1, O2, O3, O4, O5
      #else
      Options...
      #endif
      >::type
{
   typedef typename make_splaytree
      <T,
      #if !defined(AUTOBOOST_INTRUSIVE_VARIADIC_TEMPLATES)
      O1, O2, O3, O4, O5
      #else
      Options...
      #endif
      >::type   Base;
   AUTOBOOST_MOVABLE_BUT_NOT_COPYABLE(splaytree)

   public:
   typedef typename Base::value_compare      value_compare;
   typedef typename Base::value_traits       value_traits;
   typedef typename Base::iterator           iterator;
   typedef typename Base::const_iterator     const_iterator;
   typedef typename Base::reverse_iterator           reverse_iterator;
   typedef typename Base::const_reverse_iterator     const_reverse_iterator;

   //Assert if passed value traits are compatible with the type
   AUTOBOOST_STATIC_ASSERT((detail::is_same<typename value_traits::value_type, T>::value));

   explicit splaytree( const value_compare &cmp = value_compare()
                     , const value_traits &v_traits = value_traits())
      :  Base(cmp, v_traits)
   {}

   template<class Iterator>
   splaytree( bool unique, Iterator b, Iterator e
         , const value_compare &cmp = value_compare()
         , const value_traits &v_traits = value_traits())
      :  Base(unique, b, e, cmp, v_traits)
   {}

   splaytree(AUTOBOOST_RV_REF(splaytree) x)
      :  Base(::autoboost::move(static_cast<Base&>(x)))
   {}

   splaytree& operator=(AUTOBOOST_RV_REF(splaytree) x)
   {  return static_cast<splaytree &>(this->Base::operator=(::autoboost::move(static_cast<Base&>(x))));  }

   static splaytree &container_from_end_iterator(iterator end_iterator)
   {  return static_cast<splaytree &>(Base::container_from_end_iterator(end_iterator));   }

   static const splaytree &container_from_end_iterator(const_iterator end_iterator)
   {  return static_cast<const splaytree &>(Base::container_from_end_iterator(end_iterator));   }

   static splaytree &container_from_iterator(iterator it)
   {  return static_cast<splaytree &>(Base::container_from_iterator(it));   }

   static const splaytree &container_from_iterator(const_iterator it)
   {  return static_cast<const splaytree &>(Base::container_from_iterator(it));   }
};

#endif

} //namespace intrusive
} //namespace autoboost

#include <autoboost/intrusive/detail/config_end.hpp>

#endif //AUTOBOOST_INTRUSIVE_SPLAYTREE_HPP