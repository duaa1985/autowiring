// Copyright (C) 2012-2014 Leap Motion, Inc. All rights reserved.
#pragma once
#include "ConfigRegistry.h"
#include "autowiring_error.h"
#include <string>
#include <sstream>
#include STL_UNORDERED_MAP
#include MEMORY_HEADER

struct AnySharedPointer;

class AutoConfigManager:
  public ContextMember
{
public:
  AutoConfigManager();
  virtual ~AutoConfigManager();
  
private:
  std::mutex m_lock;
  std::unordered_map<std::string, AnySharedPointer> m_attributes;

public:
  /// <summary>
  /// Get a reference to where the config value is stored
  /// </summary>
  /// <remarks>
  /// This method will throw an exception if the specified name cannot be found as a configurable value
  /// in the application, or if the specified value type does not match the type expected by this field
  /// </remarks>
  AnySharedPointer& Get(const std::string& key);
  
  /// <summary>
  /// Assigns the specified value to an AnySharedPointer slot
  /// </summary>
  /// <remarks>
  /// This method will throw an exception if the specified name cannot be found as a configurable value
  /// in the application, or if the specified value type does not match the type expected by this field
  /// </remarks>
  template<class T>
  void Set(const std::string& key, const T& value) {
    std::lock_guard<std::mutex> lk(m_lock);
    
    // Iterate through all registered configs to verify the key and type match.
    bool configFound = false;
    for (auto config = g_pFirstConfigEntry; config; config = config->pFlink) {
      if (config->is(key)){
        if (!config->verifyType(typeid(T))) {
          std::stringstream ss;
          ss << "Attempting to set config '" << key << "' with incorrect type " << autowiring::demangle(typeid(T));
          throw autowiring_error(ss.str().c_str());
        }
        configFound = true;
        break;
      }
    }
    
    if (!configFound) {
      std::stringstream ss;
      ss << "No configuration found for key '" << key << "'.";
      throw autowiring_error(ss.str().c_str());
    }
    
    // Set value in this AutoConfigManager
    m_attributes[key] = AnySharedPointer(std::make_shared<T>(value));
    
    // Recurse through child contexts and set if value hasn't already been set
    for(const auto& ctxt : ContextEnumerator(GetContext())) {
      if (ctxt == GetContext())
        continue;
      
      AutowiredFast<AutoConfigManager> mgmt(ctxt);
      if(mgmt) {
        std::lock_guard<std::mutex> lk(mgmt->m_lock);
        if (mgmt->m_attributes[key]->empty())
          mgmt->m_attributes[key] = m_attributes[key];
      }
    }
  }

  /// <summary>
  /// Overload for c-style string. Converts to std::string
  /// </summary>
  void Set(const std::string& key, const char* value);

  /// <summary>
  /// Coerces the string representation of the specified field to the correct value type
  /// </summary>
  /// <remarks>
  /// This method will throw an exception if there is no string converter available on this type
  /// </remarks>
  void SetParsed(const std::string& key, const std::string& value);
};
