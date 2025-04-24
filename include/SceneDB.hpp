
//  SceneDB.hpp
//  game_engine
//
//  Created by Noah Dujovny on 1/28/25.
//

#ifndef SceneDB_hpp
#define SceneDB_hpp

#include <string>
#include <filesystem>
#include <unordered_map>
#include <memory>

#include "ReadJSON.h"
#include "TemplateDB.hpp"
#include "Actor.hpp"

class Scene {
public:
    void LoadScene(std::string& scene_name);
    static void LoadActorTemplate(const std::string& templateFp, const std::string& template_name);
    static void LoadComponent(const std::string& componentKey, const std::string& componentType, std::map<std::string, std::shared_ptr<Component>>& components, const rapidjson::Value& componentData, const std::shared_ptr<Actor>& actor);
    void Update();
    static void TemplateToSpawn(const std::string& template_name);
    static luabridge::LuaRef InstantiateActor(const std::string& template_name);
    static void RemoveActor(const luabridge::LuaRef& actor);
    static void RemoveActorById(const int actor_id);
    static std::string GetCurrent();

    ~Scene() {
        scene_actors.clear();
        new_actors_to_add.clear();
        actors_to_remove.clear();
        
        onStartComponents.clear();
        onUpdateComponents.clear();
        onLateUpdateComponents.clear();

        actor_name_map.clear();

        for (const auto& actor : actors_to_keep) {
            for (const auto& componentPair : actor->components) {
                std::shared_ptr<Component> component = componentPair.second;
                if (component->hasUpdate) {
                    onUpdateComponents.push_back(component);
                }
                if (component->hasLateUpdate) {
                    onLateUpdateComponents.push_back(component);
                }
            }
            actor_name_map[actor->name].push_back(actor);
        }

        scene_actors.insert(scene_actors.end(), actors_to_keep.begin(), actors_to_keep.end());
    }

    // Find the actor information based on actor name
    static luabridge::LuaRef FindActor(const std::string& actor_name) {
        auto it = actor_name_map.find(actor_name);
        if (it != actor_name_map.end()) {
            return luabridge::LuaRef(ComponentManager::lua_state, it->second.front().get());
        }
        return luabridge::LuaRef(ComponentManager::lua_state);
    }

    // Finds all the occurrences of actor in the scene_actors and returns them all in a lua table
    static luabridge::LuaRef FindAll(const std::string& actor_name) {
        luabridge::LuaRef actors = luabridge::newTable(ComponentManager::lua_state);
        int i = 1;
        auto it = actor_name_map.find(actor_name);
        if (it != actor_name_map.end()) {
            for (const auto& actor : it->second) {
                if (actors_to_remove.find(actor) == actors_to_remove.end()) {
                    actors[i] = luabridge::LuaRef(ComponentManager::lua_state, actor.get());
                    i++;
                }
            }
        }
        return actors;
    }

    bool SaveToFile(std::string& filename);
    static void LoadNewScene(const std::string& new_scene);
    static void DontDestroy(const luabridge::LuaRef& actor_ref);
    static bool load_new_scene;
    static std::string scene_to_load;
    static std::vector<std::shared_ptr<Actor>> scene_actors;
private:
    static std::unordered_map<std::string, std::shared_ptr<ActorTemplate>> templates;
    static std::vector<std::shared_ptr<Actor>> new_actors_to_add;
    static std::unordered_set<std::shared_ptr<Actor>> actors_to_remove;
    static std::unordered_set<std::shared_ptr<Actor>> actors_to_keep;

    static std::vector<std::shared_ptr<Component>> onStartComponents;
    static std::vector<std::shared_ptr<Component>> onUpdateComponents;
    static std::vector<std::shared_ptr<Component>> onLateUpdateComponents;
    static std::vector<std::shared_ptr<Component>> onDestroyComponents;

    static std::unordered_map<std::string, std::vector<std::shared_ptr<Actor>>> actor_name_map;

    static std::string curr_name;
};

#endif /* SceneDB_hpp */
