#pragma once
#include <typeinfo>

/// <summary>
/// Provides some static reflection support for member function pointers
/// </summary>
template<class MemFn>
struct Decompose;

template<class R, class W>
struct Decompose<R(W::*)()> {
  typedef R (W::*memType)();
  typedef void fnType();
  typedef W type;
  typedef R retType;
  static const int N = 0;

  /// <summary>
  /// Returns an array of length N+1 of argument types on the bound type
  /// </summary>
  /// <remarks>
  /// The last entry in this list is guaranteed to be nullptr.  The list's actual size is
  /// one larger than the number of arguments, in order to allow a proper return for a zero-
  /// argument function pointer.
  /// </remarks>
  static const std::type_info* (&Enumerate(void))[N + 1] {
    static const std::type_info* ti[] = {nullptr};
    return ti;
  }

  /// <summary>
  /// Performs a call on the specified object with the specified member function
  /// <summary>
  /// <remarks>
  /// The passed type object, the last parameter, must be a type repository.  It must support
  /// a method of the following form:
  ///
  /// template<class T>
  /// const T& Get();
  ///
  /// The method must return the object of the desired type, or else it must throw an exception.
  /// It is the caller's responsibility to ensure that all of the necessary types are available
  /// before this call is made, in order to prevent such an exception from being thrown.
  /// </remarks>
  template<class T>
  static void Call(W* pObj, memType memfn, const T&) {
    (pObj->*memfn)();
  }
};

template<class R, class W, class Arg1>
struct Decompose<R(W::*)(Arg1)> {
  typedef R(W::*memType)(Arg1);
  typedef void fnType(Arg1);
  typedef W type;
  typedef R retType;
  static const int N = 1;

  static const std::type_info* (&Enumerate(void))[N + 1] {
    static const std::type_info* ti[] = {&typeid(Arg1), nullptr};
    return ti;
  }

  template<class T>
  static void Call(W* pObj, memType memfn, const T& repo) {
    (pObj->*memfn)(repo.Get<Arg1>());
  }
};

template<class R, class W, class Arg1, class Arg2>
struct Decompose<R(W::*)(Arg1, Arg2)> {
  typedef R(W::*memType)(Arg1, Arg2);
  typedef void fnType(Arg1, Arg2);
  typedef W type;
  typedef R retType;
  static const int N = 2;

  static const std::type_info* (&Enumerate(void))[N + 1] {
    static const std::type_info* ti[] = {&typeid(Arg1), &typeid(Arg2), nullptr};
    return ti;
  }

  template<class T>
  static void Call(W* pObj, memType memfn, const T& repo) {
    (pObj->*memfn)(repo.Get<Arg1>(), repo.Get<Arg2>());
  }
};

template<class R, class W, class Arg1, class Arg2, class Arg3>
struct Decompose<R(W::*)(Arg1, Arg2, Arg3)> {
  typedef R(W::*memType)(Arg1, Arg2, Arg3);
  typedef void fnType(Arg1, Arg2, Arg3);
  typedef W type;
  typedef R retType;
  static const int N = 3;

  static const std::type_info* (&Enumerate(void))[N + 1] {
    static const std::type_info* ti[] = {&typeid(Arg1), &typeid(Arg2), &typeid(Arg3), nullptr};
    return ti;
  }

  template<class T>
  static void Call(W* pObj, memType memfn, const T& repo) {
    (pObj->*memfn)(repo.Get<Arg1>(), repo.Get<Arg2>(), repo.Get<Arg3>());
  }
};

template<class R, class W, class Arg1, class Arg2, class Arg3, class Arg4>
struct Decompose<R(W::*)(Arg1, Arg2, Arg3, Arg4)> {
  typedef R(W::*memType)(Arg1, Arg2, Arg3, Arg4);
  typedef void fnType(Arg1, Arg2, Arg3, Arg4);
  typedef W type;
  typedef R retType;
  static const int N = 4;

  static const std::type_info* (&Enumerate(void))[N + 1] {
    static const std::type_info* ti[] = {&typeid(Arg1), &typeid(Arg2), &typeid(Arg3), &typeid(Arg4), nullptr};
    return ti;
  }

  template<class T>
  static void Call(W* pObj, memType memfn, const T& repo) {
    (pObj->*memfn)(repo.Get<Arg1>(), repo.Get<Arg2>(), repo.Get<Arg3>(), repo.Get<Arg4>());
  }
};