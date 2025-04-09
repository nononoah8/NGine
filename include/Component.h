#ifndef COMPONENT_H
#define COMPONENT_H

#include <memory>
#include <string>

#include "ComponentManager.h"
#include "Lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"

class Actor;

class Component {
public:
  explicit Component() 
  : componentRef(std::make_shared<luabridge::LuaRef>(luabridge::LuaRef(ComponentManager::lua_state))), 
    type(""), 
    hasStart(false), 
    hasUpdate(false), 
    hasLateUpdate(false),
    enabled(true),
    added(false),
    callOnStart(false),
    destroyed(false),
    key(""),
    addedStart(false),
    addedUpdate(false),
    addedLate(false),
    isRb(false),
    hasDestroy(false),
    isPs(false) {}

  bool IsEnabled();

  void SetComponentProps();

  std::shared_ptr<luabridge::LuaRef> componentRef;
  std::string type;
  std::string key;
  std::shared_ptr<Actor> actor;

  bool hasStart;
  bool hasUpdate;
  bool hasLateUpdate;
  bool hasDestroy;
  bool enabled;
  bool added;
  bool callOnStart;
  bool destroyed;
  bool addedStart;
  bool addedUpdate;
  bool addedLate;
  bool isRb;
  bool isPs; // is particle system
};

#endif
