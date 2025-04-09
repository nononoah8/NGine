#include "Headers/ComponentManager.h"

lua_State* ComponentManager::lua_state = nullptr;
int ComponentManager::num_runtime_components;


void ComponentManager::Initialize() {
  InitializeState();
  num_runtime_components = 0;
  // InitializeFunctions();
  // InitializeComponents();
}

void ComponentManager::InitializeState() {
  // Initialize the state
  lua_state = luaL_newstate();
  luaL_openlibs(lua_state);
}