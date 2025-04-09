#include "ComponentDB.hpp"
#include "ComponentManager.h"
#include "Component.h"

#include "Actor.hpp"
#include "SceneDB.hpp"
#include "Application.hpp"
#include "Input.h"
#include "TextDB.h"
#include "AudioDB.h"
#include "EventSystem.h"

#include <filesystem>
#include <string>

std::unordered_map<std::string, std::shared_ptr<Component>> ComponentDB::components;

void ComponentDB::Init() {
    // Added debug statements - Debug.Log / Debug.Error
    luabridge::getGlobalNamespace(ComponentManager::lua_state)
    .beginNamespace("Debug")
    .addFunction("Log", ComponentDB::CppLog)
    // .addFunction("LogError", ComponentDB::CppError)
    .endNamespace();

    // Add Other Actor API
    luabridge::getGlobalNamespace(ComponentManager::lua_state)
    .beginClass<Actor>("Actor")
    .addFunction("GetName", &Actor::GetName)
    .addFunction("GetID", &Actor::GetID)
    .addFunction("GetComponentByKey", &Actor::GetComponentByKey)
    .addFunction("GetComponent", &Actor::GetComponent)
    .addFunction("GetComponents", &Actor::GetComponents)
    .addFunction("AddComponent", &Actor::AddComponent)
    .addFunction("RemoveComponent", &Actor::RemoveComponent)
    .endClass();

    // Add Actor API
    luabridge::getGlobalNamespace(ComponentManager::lua_state)
    .beginNamespace("Actor")
    .addFunction("Find", &Scene::FindActor)
    .addFunction("FindAll", &Scene::FindAll)
    .addFunction("Instantiate", &Scene::InstantiateActor)
    .addFunction("Destroy", &Scene::RemoveActor)
    .endNamespace();

    // Add Application Scripting API
    luabridge::getGlobalNamespace(ComponentManager::lua_state)
    .beginNamespace("Application")
    .addFunction("GetFrame", &Application::GetFrame)
    .addFunction("Quit", &Application::Quit)
    .addFunction("Sleep", &Application::Sleep)
    .addFunction("OpenURL", &Application::OpenUrl)
    .endNamespace();

    // Add vec2 Class to Lua
    luabridge::getGlobalNamespace(ComponentManager::lua_state)
    .beginClass<glm::vec2>("vec2")
    .addProperty("x", &glm::vec2::x)
    .addProperty("y", &glm::vec2::y)
    .endClass();

    // Add Input Manager Scripting API
    luabridge::getGlobalNamespace(ComponentManager::lua_state)
    .beginNamespace("Input")
    .addFunction("GetKey", &Input::GetKeyByName)
    .addFunction("GetKeyDown", &Input::GetKeyDownByName)
    .addFunction("GetKeyUp", &Input::GetKeyUpByName)
    .addFunction("GetMousePosition", &Input::GetMousePosition)
    .addFunction("GetMouseButton", &Input::GetMouseButton)
    .addFunction("GetMouseButtonDown", &Input::GetMouseButtonDown)
    .addFunction("GetMouseButtonUp", &Input::GetMouseButtonUp)
    .addFunction("GetMouseScrollDelta", &Input::GetMouseScrollDelta)
    .addFunction("HideCursor", &Input::HideCursor)
    .addFunction("ShowCursor", &Input::ShowCursor)
    .endNamespace();

    // Add Text manager
    luabridge::getGlobalNamespace(ComponentManager::lua_state)
    .beginNamespace("Text")
    .addFunction("Draw", TextDB::Draw)
    .endNamespace();

    // Add volume manager
    luabridge::getGlobalNamespace(ComponentManager::lua_state)
    .beginNamespace("Audio")
    .addFunction("Play", &AudioDB::PlayChannel)
    .addFunction("Halt", &AudioDB::HaltChannel)
    .addFunction("SetVolume", &AudioDB::SetVolume)
    .endNamespace();

    // Add Image manager
    luabridge::getGlobalNamespace(ComponentManager::lua_state)
    .beginNamespace("Image")
    .addFunction("DrawUI", &ImageDB::DrawUI)
    .addFunction("DrawUIEx", &ImageDB::DrawUIEx)
    .addFunction("Draw", &ImageDB::Draw)
    .addFunction("DrawEx", &ImageDB::DrawEx)
    .addFunction("DrawPixel", &ImageDB::DrawPixel)
    .endNamespace();

    // Add Camera manager
    luabridge::getGlobalNamespace(ComponentManager::lua_state)
    .beginNamespace("Camera")
    .addFunction("SetPosition", &Renderer::SetCamPos)
    .addFunction("GetPositionX", &Renderer::GetCamPosX)
    .addFunction("GetPositionY", &Renderer::GetCamPosY)
    .addFunction("SetZoom", &Renderer::SetCamZoom)
    .addFunction("GetZoom", &Renderer::GetZoom)
    .endNamespace();

    // Add Scene manager
    luabridge::getGlobalNamespace(ComponentManager::lua_state)
    .beginNamespace("Scene")
    .addFunction("Load", &Scene::LoadNewScene)
    .addFunction("GetCurrent", &Scene::GetCurrent)
    .addFunction("DontDestroy", &Scene::DontDestroy)
    .endNamespace();
    
    // Add "Vector2" datatype
    luabridge::getGlobalNamespace(ComponentManager::lua_state)
    .beginClass<b2Vec2>("Vector2")
    .addConstructor<void(*) (float, float)>()
    .addProperty("x", &b2Vec2::x)
    .addProperty("y", &b2Vec2::y)
    .addFunction("Normalize", &b2Vec2::Normalize)
    .addFunction("Length", &b2Vec2::Length)
    .addFunction("__add", &b2Vec2::operator_add)
    .addFunction("__sub", &b2Vec2::operator_sub)
    .addFunction("__mul", &b2Vec2::operator_mult)
    .addStaticFunction("Distance", &b2Vec2::Distance)
    .addStaticFunction("Dot", &b2Vec2::Dot)
    .endClass();

    // Add Rigidbody
    luabridge::getGlobalNamespace(ComponentManager::lua_state)
    .beginClass<Rigidbody>("Rigidbody")
    .addProperty("x", &Rigidbody::x)
    .addProperty("y", &Rigidbody::y)
    .addProperty("body_type", &Rigidbody::body_type)
    .addProperty("precise", &Rigidbody::precise)
    .addProperty("gravity_scale", &Rigidbody::gravity_scale)
    .addProperty("density", &Rigidbody::density)
    .addProperty("angular_friction", &Rigidbody::angular_friction)
    .addProperty("rotation", &Rigidbody::rotation)
    .addProperty("has_collider", &Rigidbody::has_collider)
    .addProperty("has_trigger", &Rigidbody::has_trigger)
    .addProperty("key", &Rigidbody::key)
    .addProperty("type", &Rigidbody::type)
    .addFunction("GetPosition", &Rigidbody::GetPosition)
    .addFunction("GetRotation", &Rigidbody::GetRotation)
    .addFunction("AddForce", &Rigidbody::AddForce)
    .addFunction("SetVelocity", &Rigidbody::SetVelocity)
    .addFunction("SetPosition", &Rigidbody::SetPosition)
    .addFunction("SetRotation", &Rigidbody::SetRotation)
    .addFunction("SetAngularVelocity", &Rigidbody::SetAngularVelocity)
    .addFunction("SetGravityScale", &Rigidbody::SetGravityScale)
    .addFunction("SetUpDirection", &Rigidbody::SetUpDirection)
    .addFunction("SetRightDirection", &Rigidbody::SetRightDirection)
    .addFunction("GetVelocity", &Rigidbody::GetVelocity)
    .addFunction("GetAngularVelocity", &Rigidbody::GetAngularVelocity)
    .addFunction("GetGravityScale", &Rigidbody::GetGravityScale)
    .addFunction("GetUpDirection", &Rigidbody::GetUpDirection)
    .addFunction("GetRightDirection", &Rigidbody::GetRightDirection)
    .endClass();
    
    luabridge::getGlobalNamespace(ComponentManager::lua_state)
    .beginClass<Collision>("Collision")
    .addProperty("other", &Collision::other)
    .addProperty("point", &Collision::point)
    .addProperty("relative_velocity", &Collision::relative_velocity)
    .addProperty("normal", &Collision::normal)
    .endClass();

    
    luabridge::getGlobalNamespace(ComponentManager::lua_state)
    .beginNamespace("Physics")
    .addFunction("Raycast", &RaycastCallback::Raycast)
    .addFunction("RaycastAll", &RaycastCallback::RaycastAll)
    .endNamespace();

    luabridge::getGlobalNamespace(ComponentManager::lua_state)
    .beginClass<HitResult>("HitResult")
    .addProperty("actor", &HitResult::actor)
    .addProperty("point", &HitResult::point)
    .addProperty("is_trigger", &HitResult::is_trigger)
    .addProperty("normal", &HitResult::normal)
    .endClass();

    luabridge::getGlobalNamespace(ComponentManager::lua_state)
    .beginNamespace("Event")
    .addFunction("Publish", &EventSystem::Publish)
    .addFunction("Subscribe", &EventSystem::Subscribe)
    .addFunction("Unsubscribe", &EventSystem::Unsubscribe)
    .endNamespace();

    luabridge::getGlobalNamespace(ComponentManager::lua_state)
    .beginClass<ParticleSystem>("ParticleSystem")
    .addProperty("x", &ParticleSystem::x)
    .addProperty("y", &ParticleSystem::y)
    .addProperty("start_color_r", &ParticleSystem::start_color_r)
    .addProperty("start_color_g", &ParticleSystem::start_color_g)
    .addProperty("start_color_b", &ParticleSystem::start_color_b)
    .addProperty("start_color_a", &ParticleSystem::start_color_a)
    .addProperty("enabled", &ParticleSystem::getEnabled, &ParticleSystem::setEnabled)
    .addProperty("frames_between_bursts", &ParticleSystem::frames_between_bursts)
    .addProperty("burst_quantity", &ParticleSystem::burst_quantity)
    .addProperty("duration_frames", &ParticleSystem::duration_frames)
    .addFunction("Stop", &ParticleSystem::Stop)
    .addFunction("Play", &ParticleSystem::Play)
    .addFunction("Burst", &ParticleSystem::Burst)
    .endClass();
}

std::shared_ptr<Component> ComponentDB::AddComponent(std::string component_name) {
    if(components.find(component_name) != components.end()) {
      return components[component_name];
    }

    std::string fp = "resources/component_types/" + component_name + ".lua";

    if (!std::filesystem::exists(fp)) {
      std::cout << "error: failed to locate component " << component_name;
      exit(0);
    }

    if(luaL_dofile(ComponentManager::lua_state, fp.c_str()) != LUA_OK) {
      std::cout << "problem with lua file " << component_name;
      exit(0);
    }

    std::shared_ptr<luabridge::LuaRef> componentRef = std::make_shared<luabridge::LuaRef>(luabridge::getGlobal(ComponentManager::lua_state, component_name.c_str()));
    std::shared_ptr<Component> component = std::make_shared<Component>();
    component->componentRef = componentRef;
    component->type = component_name;
    components.insert({ component_name, component });

    return component;
}

void ComponentDB::EstablishInheritance(luabridge::LuaRef& instance_table, luabridge::LuaRef& parent_table) {
    // Create a new metatable for the child component
    luabridge::LuaRef new_metatable = luabridge::newTable(ComponentManager::lua_state);
    new_metatable["__index"] = parent_table;

    // Push the instance table and assign its metatable
    instance_table.push(ComponentManager::lua_state);
    new_metatable.push(ComponentManager::lua_state);
    lua_setmetatable(ComponentManager::lua_state, -2);
    lua_pop(ComponentManager::lua_state, 1);
}
