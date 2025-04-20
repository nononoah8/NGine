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
#include "Renderer.h"

#include "GameObjectDB.h"
#include "GameObject.h"
#include "LightComponent.h"

#include <filesystem>
#include <string>

std::unordered_map<std::string, std::shared_ptr<Component>> ComponentDB::components;

static glm::vec3 vec3_add(const glm::vec3& v1, const glm::vec3& v2) { return v1 + v2; }
static glm::vec3 vec3_sub(const glm::vec3& v1, const glm::vec3& v2) { return v1 - v2; }
static glm::vec3 vec3_mul(const glm::vec3& v, float scalar) { return v * scalar; }

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

    // // Add Camera manager
    luabridge::getGlobalNamespace(ComponentManager::lua_state)
    .beginNamespace("Camera")
    .addFunction("SetPosition", &Renderer::SetCamPos)
    .addFunction("GetPosition", &Renderer::GetCamPos)
    .addFunction("GetPositionX", &Renderer::GetCamPosX)
    .addFunction("GetPositionY", &Renderer::GetCamPosY)
    .addFunction("GetPositionZ", &Renderer::GetCamPosZ)
    .addFunction("SetZoom", &Renderer::SetCamZoom)
    .addFunction("GetZoom", &Renderer::GetZoom)
    .addFunction("GetFront", &Renderer::GetCameraFront)
    .addFunction("SetFront", &Renderer::SetCameraFront) 
    .addFunction("GetUp", &Renderer::GetCameraUp)
    .addFunction("SetUp", &Renderer::SetCameraUp)
    .addFunction("GetYaw", &Renderer::GetCameraYaw)
    .addFunction("SetYaw", &Renderer::SetCameraYaw)
    .addFunction("GetPitch", &Renderer::GetCameraPitch)
    .addFunction("SetPitch", &Renderer::SetCameraPitch)
    .addFunction("UpdateDirection", &Renderer::UpdateCameraDirection)
    .endNamespace();


    // Add Vector3 datatype
    //TODO: Add functions to this class.
    luabridge::getGlobalNamespace(ComponentManager::lua_state)
    .beginClass<glm::vec3>("Vector3")
    .addConstructor<void(*) (float, float, float)>()
    .addProperty("x", &glm::vec3::x)
    .addProperty("y", &glm::vec3::y)
    .addProperty("z", &glm::vec3::z)
    .endClass();

    luabridge::getGlobalNamespace(ComponentManager::lua_state)
    .beginNamespace("Model")
    .addFunction("DrawCube", &GameObjectDB::CreateCube)
    .addFunction("DrawSphere", &GameObjectDB::CreateSphere)
    .addFunction("DrawModel", &GameObjectDB::LoadModel)
    .endNamespace();

    luabridge::getGlobalNamespace(ComponentManager::lua_state)
    .beginClass<std::shared_ptr<GameObject>>("GameObjectPtr")
    .endClass();

    // Add Scene manager
    luabridge::getGlobalNamespace(ComponentManager::lua_state)
    .beginNamespace("Scene")
    .addFunction("Load", &Scene::LoadNewScene)
    .addFunction("GetCurrent", &Scene::GetCurrent)
    .addFunction("DontDestroy", &Scene::DontDestroy)
    .endNamespace();

    luabridge::getGlobalNamespace(ComponentManager::lua_state)
    .beginNamespace("Event")
    .addFunction("Publish", &EventSystem::Publish)
    .addFunction("Subscribe", &EventSystem::Subscribe)
    .addFunction("Unsubscribe", &EventSystem::Unsubscribe)
    .endNamespace();

    // LightComopnent 
    luabridge::getGlobalNamespace(ComponentManager::lua_state)
    .beginClass<LightComponent>("LightComponent")
    // Light properties getters
    .addFunction("GetType", &LightComponent::GetType)
    .addFunction("GetPosition", &LightComponent::GetPosition)
    .addFunction("GetDirection", &LightComponent::GetDirection)
    .addFunction("GetColor", &LightComponent::GetColor)
    .addFunction("GetIntensity", &LightComponent::GetIntensity)
    .addFunction("GetConstant", &LightComponent::GetConstant)
    .addFunction("GetLinear", &LightComponent::GetLinear)
    .addFunction("GetQuadratic", &LightComponent::GetQuadratic)
    .addFunction("GetInnerCutoff", &LightComponent::GetInnerCutoff)
    .addFunction("GetOuterCutoff", &LightComponent::GetOuterCutoff)
    .addFunction("GetName", &LightComponent::GetName)
    // Light properties setters
    .addFunction("SetType", &LightComponent::SetType)
    .addFunction("SetPosition", &LightComponent::SetPosition)
    .addFunction("SetDirection", &LightComponent::SetDirection)
    .addFunction("SetColor", &LightComponent::SetColor)
    .addFunction("SetIntensity", &LightComponent::SetIntensity)
    .addFunction("SetConstant", &LightComponent::SetConstant)
    .addFunction("SetLinear", &LightComponent::SetLinear)
    .addFunction("SetQuadratic", &LightComponent::SetQuadratic)
    .addFunction("SetInnerCutoff", &LightComponent::SetInnerCutoff)
    .addFunction("SetOuterCutoff", &LightComponent::SetOuterCutoff)
    .addFunction("SetName", &LightComponent::SetName)
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
