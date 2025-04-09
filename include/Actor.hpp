//
//  Actor.hpp
//  game_engine
//
//  Created by Noah Dujovny on 1/28/25.
//
#ifndef ACTOR_HPP
#define ACTOR_HPP

#include <string>
#include <optional>
#include <unordered_set>
#include <memory>
#include <vector>
#include <map>
#include <algorithm>

#include "Component.h"
#include "ComponentDB.hpp"
#include "ComponentManager.h"
#include "Lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"

//TODO: CLean up!!!!!

class Actor : public std::enable_shared_from_this<Actor> {
public:
    int id = -1;
    std::string name = "";
    std::string template_name = "";
    bool created = false;
    bool dontDestroy=false;

    static inline int actor_uid = 0;
    std::map<std::string, std::shared_ptr<Component>> components;
    std::unordered_map<std::string, std::vector<std::shared_ptr<Component>>> componentsByType;

    Actor(std::string name, std::string template_name="") : name(name), template_name(template_name) {
        id = actor_uid;
        actor_uid++;
    }

    std::string GetName() {
        return name;
    }

    int GetID() {
        return id;
    }

    void InjectConvenienceReferences(std::shared_ptr<Component> component) {
        (*component->componentRef)["actor"] = this;
    }

    luabridge::LuaRef GetComponentByKey(const std::string& key) {
        auto it = components.find(key);
        if(it != components.end()) {
            // if(it->second->isRb) {
            //     return luabridge::LuaRef(ComponentManager::lua_state, std::static_pointer_cast<Rigidbody>(it->second).get());
            // }else if(it->second->isPs) {
            //     return luabridge::LuaRef(ComponentManager::lua_state, std::static_pointer_cast<ParticleSystem>(it->second).get());
            // }
            return *(it->second->componentRef);
        }
        return luabridge::LuaRef(ComponentManager::lua_state);
    }

    luabridge::LuaRef GetComponent(const std::string& type) {
        auto it = componentsByType.find(type);
        if (it != componentsByType.end() && !it->second.empty()) {
            // if(it->second.front()->isRb) {
            //     return luabridge::LuaRef(ComponentManager::lua_state, std::static_pointer_cast<Rigidbody>(it->second.front()).get());
            // }else if(it->second.front()->isPs) {
            //     return luabridge::LuaRef(ComponentManager::lua_state, std::static_pointer_cast<ParticleSystem>(it->second.front()).get());
            // }
            return *(it->second.front()->componentRef);
        }
        return luabridge::LuaRef(ComponentManager::lua_state);
    }

    luabridge::LuaRef GetComponents(const std::string& type) {
        auto it = componentsByType.find(type);
        if (it != componentsByType.end()) {
            luabridge::LuaRef returnTable = luabridge::newTable(ComponentManager::lua_state);
            int idx = 1;
            for (const auto& component : it->second) {
                // if(component->isRb) {
                //     returnTable[idx++] = luabridge::LuaRef(ComponentManager::lua_state, std::static_pointer_cast<Rigidbody>(component).get());
                // }else if(component->isPs) {
                //     returnTable[idx++] = luabridge::LuaRef(ComponentManager::lua_state, std::static_pointer_cast<ParticleSystem>(component).get());
                // }else {
                // }
                returnTable[idx++] = *(component->componentRef);
            }
            return returnTable;
        }
        return luabridge::LuaRef(ComponentManager::lua_state);
    }

    luabridge::LuaRef AddComponent(const std::string& type_name) {
        // set the key to be "rn" where r is runtime added, and n is global counter
        std::string key = "r" + std::to_string(ComponentManager::num_runtime_components);
        std::shared_ptr<Component> newComponent = std::make_shared<Component>();
        luabridge::LuaRef componentInstance = luabridge::newTable(ComponentManager::lua_state);

        // if(type_name == "Rigidbody") {
        //     auto rigidbody = std::make_shared<Rigidbody>();
        //     // rigidbody->isRb = true;
        //     rigidbody->hasStart = true;
        //     rigidbody->key = key;
        //     rigidbody->type = "Rigidbody";
        //     newComponent = rigidbody;
        // }else if(type_name == "ParticleSystem") {
        //     auto particlesystem = std::make_shared<ParticleSystem>();
        //     particlesystem->isPs = true;
        //     particlesystem->hasStart = true;
        //     particlesystem->hasUpdate = true;
        //     particlesystem->key = key;
        //     particlesystem->type = "ParticleSystem";
        //     newComponent = particlesystem;
        // } else {
        // }
        // Ceate the new component.
        std::shared_ptr<Component> baseComponent = ComponentDB::AddComponent(type_name);
        ComponentDB::EstablishInheritance(componentInstance, *(baseComponent->componentRef));

        newComponent->SetComponentProps();

        newComponent->componentRef = std::make_shared<luabridge::LuaRef>(componentInstance);
        newComponent->type = type_name;
        newComponent->actor = shared_from_this();
        newComponent->key = key;

        // Set the new component's properties in the Lua table
        (*newComponent->componentRef)["type"] = type_name;
        (*newComponent->componentRef)["enabled"] = true;
        (*newComponent->componentRef)["key"] = key;

        components[key] = newComponent;
        componentsByType[type_name].push_back(newComponent);

        ComponentManager::components_to_add.push_back(newComponent);

        // Inject convenience references
        InjectConvenienceReferences(newComponent);

        newComponent->actor = shared_from_this();

        // increment the global 'n', global int counter of num times addcomponent has been called.
        ComponentManager::num_runtime_components++;

        // retrun the luaref of the newly created component
        // if(newComponent->isRb) {
        //     return luabridge::LuaRef(ComponentManager::lua_state, std::static_pointer_cast<Rigidbody>(newComponent).get());
        // }else {
        // }
        return componentInstance;
    }

    void RemoveComponent(luabridge::LuaRef component) {
        // std::cout << component["x"] << std::endl;

        // Get the component.
        std::string key = component["key"].tostring();
        auto it = components.find(key);

        if (it != components.end()) {
            // Disable it - It can't do any more lifecycle functions
            (*it->second->componentRef)["enabled"] = false;
            it->second->enabled = false;

            // Remove it from the componentsByType map
            auto& vec = componentsByType[it->second->type];
            vec.erase(std::remove(vec.begin(), vec.end(), it->second), vec.end());

            ComponentManager::components_to_remove.push_back(it->second);

            // Remove it from the map
            components.erase(it);
        }
    }
};

#endif
