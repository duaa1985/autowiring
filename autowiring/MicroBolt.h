// Copyright (C) 2012-2014 Leap Motion, Inc. All rights reserved.
#pragma once
#include "Bolt.h"
#include "CoreContext.h"
#include "ContextEnumerator.h"

/// <summary>
/// Causes the class inheriting from this definition to be bolted to the specified contexts
/// </summary>
template<typename... Sigils>
struct Boltable {};

/// <summary>
/// A default bolt type which will insert the specified type into a matching context
/// </summary>
template<typename T, typename... Sigils>
struct MicroBolt:
  public Bolt<Sigils...>
{
public:
  MicroBolt(void) {
    // ASSERT: Inject<T>() is idempotent.
    // NOTE: Injection of T into all matching contexts may result in
    // multiple calls to Inject<T>() if a matching context
    // is created during traversal.
    for(auto findSigil : {ContextEnumeratorT<Sigils>(CoreContext::CurrentContext())...})
      for(auto context : findSigil)
        context->template Inject<T>();
  }
  void ContextCreated(void) override;
};

template<typename T, typename... Sigils>
void MicroBolt<T, Sigils...>::ContextCreated(void) {
  CoreContext::InjectCurrent<T>();
}
