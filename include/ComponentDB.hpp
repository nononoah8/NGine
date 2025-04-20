#ifndef COMPONENTDB_HPP
#define COMPONENTDB_HPP

#include "Lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"

#include "Component.h"
#include <memory>
#include <string>

class ComponentDB {
public:
  static void Init();

  static std::shared_ptr<Component> AddComponent(std::string component_name);
  static void EstablishInheritance(luabridge::LuaRef& instance_table, luabridge::LuaRef& parent_table);

  static void CppLog(const std::string& message) {
    std::cout << message << std::endl;
  }

  static void CppError(const std::string& message) {
    std::cout << message << std::endl;
  }
private:
  static std::unordered_map<std::string, std::shared_ptr<Component>> components;
};

#endif