#ifndef COMPONENTMANAGER_H
#define COMPONENTMANAGER_H

//#include "Headers/Component.h"
#include "Lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"
#include <memory>
#include <vector>

class Component;

class ComponentManager {
  public:
    static void Initialize();
    static lua_State* lua_state;
    static int num_runtime_components;
    static std::vector<std::shared_ptr<Component>> components_to_add;
    static std::vector<std::shared_ptr<Component>> components_to_remove;
  private:
    static void InitializeState();
    static void InitializeFunctions();
    static void InitializeComponents();
};

#endif
