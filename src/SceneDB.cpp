//
//  SceneDB.cpp
//  game_engine
//
//  Created by Noah Dujovny on 2/10/25.
//

#include "SceneDB.hpp"
#include "AudioDB.h"
#include "ComponentManager.h"
#include "ComponentDB.hpp"

void ReportError(std::string& actor_name, const luabridge::LuaException& e);
std::shared_ptr<Component> LoadExistingComponent(std::shared_ptr<Component> component);
void SetComponentProperties(luabridge::LuaRef& componentInstance, const rapidjson::Value& componentData);

// Adding all of the static member variables so I can use them in the program
std::vector<std::shared_ptr<Actor>> Scene::scene_actors;
std::unordered_map<std::string, std::shared_ptr<ActorTemplate>> Scene::templates;
std::vector<std::shared_ptr<Actor>> Scene::new_actors_to_add;
std::unordered_set<std::shared_ptr<Actor>> Scene::actors_to_remove;
std::unordered_set<std::shared_ptr<Actor>> Scene::actors_to_keep;
std::string Scene::scene_to_load;
bool Scene::load_new_scene;
std::string Scene::curr_name;

std::vector<std::shared_ptr<Component>> Scene::onStartComponents;
std::vector<std::shared_ptr<Component>> Scene::onUpdateComponents;
std::vector<std::shared_ptr<Component>> Scene::onLateUpdateComponents;
std::vector<std::shared_ptr<Component>> Scene::onDestroyComponents;

std::unordered_map<std::string, std::vector<std::shared_ptr<Actor>>> Scene::actor_name_map;

std::vector<std::shared_ptr<Component>> ComponentManager::components_to_add;
std::vector<std::shared_ptr<Component>> ComponentManager::components_to_remove;


/*
    * Load a scene
    * This function will load a scene from a file and store it in the scene_actors vector
*/
void Scene::LoadScene(std::string& scene_name) {
  rapidjson::Document doc;

  curr_name = scene_name;
  
  std::string initial_scene_fp = "resources/scenes/" + scene_name + ".scene";

  if(!std::filesystem::exists(initial_scene_fp)){
      std::cout << "error: scene " << scene_name << " is missing";
      exit(0);
  }

  ReadJsonFile(initial_scene_fp, doc);
  const rapidjson::Value& actors = doc["actors"];

  scene_actors.reserve(actors.Size());

  // Make a vector / resize a vector to be of size actors.Size()
  for(rapidjson::SizeType i = 0; i < actors.Size(); ++i) {
        const rapidjson::Value& actor = actors[i];
        
        std::string actor_template_name = actor.HasMember("template") ? actor["template"].GetString() : "";

        // Adds the actor template to the hash table.
        auto actorTemplate = templates.find(actor_template_name);
        if(actor_template_name != "" && actorTemplate == templates.end()) {
            // Check if the template file exists
            TemplateToSpawn(actor_template_name);
        }

        std::string actor_name = "";

        // It's using a template
        auto actor_template = templates.find(actor_template_name);
        if(actor_template_name != "" && actor_template != templates.end()) {
            // set all the values to the template values.
            actor_name = actor_template->second->name;
        }

        actor_name = getJsonStringOrDefault(actor, "name", actor_name);

        std::shared_ptr<Actor> newActor = std::make_shared<Actor>(actor_name);


        if (actor_template != templates.end()) {
            for (const auto& componentPair : actor_template->second->components) {
                newActor->components[componentPair.first] = LoadExistingComponent(componentPair.second);
                newActor->components[componentPair.first]->SetComponentProps();
                newActor->componentsByType[componentPair.second->type].push_back(newActor->components[componentPair.first]);
                newActor->components[componentPair.first]->actor = newActor;
            }
        }

        if (actor.HasMember("components")) {
            const rapidjson::Value& components = actor["components"];

            for (auto& component : components.GetObject()) {
                std::string componentKey = component.name.GetString();
                
                if(component.value.HasMember("type")) {
                    std::string componentType = component.value["type"].GetString();
    
                    LoadComponent(componentKey, componentType, newActor->components, component.value, newActor);
    
                    newActor->components[componentKey]->SetComponentProps();
                    newActor->componentsByType[componentType].push_back(newActor->components[componentKey]);
                    newActor->components[componentKey]->key = componentKey;

                    if(newActor->components[componentKey]->hasStart || newActor->components[componentKey]->isRb) {
                        onStartComponents.push_back(newActor->components[componentKey]);
                    }
                    if(newActor->components[componentKey]->hasUpdate) {
                        onUpdateComponents.push_back(newActor->components[componentKey]);
                    }
                    if(newActor->components[componentKey]->hasLateUpdate) {
                        onLateUpdateComponents.push_back(newActor->components[componentKey]);
                    }
                } else {
                    // If the component doesn't have a type, override the current properties of the current component
                    LoadComponent(componentKey, "", newActor->components, component.value, newActor);
                    newActor->components[componentKey]->SetComponentProps();
                    newActor->components[componentKey]->key = componentKey;
                    if(newActor->components[componentKey]->hasStart || newActor->components[componentKey]->isRb) {
                        onStartComponents.push_back(newActor->components[componentKey]);
                    }
                    if(newActor->components[componentKey]->hasUpdate) {
                        onUpdateComponents.push_back(newActor->components[componentKey]);
                    }
                    if(newActor->components[componentKey]->hasLateUpdate) {
                        onLateUpdateComponents.push_back(newActor->components[componentKey]);
                    }
                }
            }
        } else if(actor_template != templates.end()) {
            for (const auto& componentPair : newActor->components) {
                componentPair.second->SetComponentProps();
                componentPair.second->key = componentPair.first;
                componentPair.second->actor = newActor;
                if(componentPair.second->hasStart || componentPair.second->isRb) {
                    onStartComponents.push_back(componentPair.second);
                }
                if(componentPair.second->hasUpdate) {
                    onUpdateComponents.push_back(componentPair.second);
                }
                if(componentPair.second->hasLateUpdate) {
                    onLateUpdateComponents.push_back(componentPair.second);
                }
            }
        }

        // loop through all of the actor's components
        for(auto& component : newActor->components) {
            std::shared_ptr<Component> componentInstance = component.second;
            if(!component.second->isRb){
                newActor->InjectConvenienceReferences(componentInstance);
            }
        }

        actor_name_map[newActor->name].push_back(newActor);
        scene_actors.push_back(std::move(newActor));
    }

    // Sort the components by key and actor id
    std::sort(onStartComponents.begin(), onStartComponents.end(), [](const std::shared_ptr<Component>& a, const std::shared_ptr<Component>& b) {
        if (a->actor->id == b->actor->id) {
            return a->key < b->key;
        }
        return a->actor->id < b->actor->id;
    });
    
    std::sort(onUpdateComponents.begin(), onUpdateComponents.end(), [](const std::shared_ptr<Component>& a, const std::shared_ptr<Component>& b) {
        if (a->actor->id == b->actor->id) {
            return a->key < b->key;
        }
        return a->actor->id < b->actor->id;
    });
    
    std::sort(onLateUpdateComponents.begin(), onLateUpdateComponents.end(), [](const std::shared_ptr<Component>& a, const std::shared_ptr<Component>& b) {
        if (a->actor->id == b->actor->id) {
            return a->key < b->key;
        }
        return a->actor->id < b->actor->id;
    });
}

/*
    * Load a component
    * This function will load a component from a file and store it in the actor
*/
void Scene::LoadComponent(const std::string& componentKey, const std::string& componentType, std::map<std::string, std::shared_ptr<Component>>& components, const rapidjson::Value& componentData, const std::shared_ptr<Actor>& actor = nullptr) {
    // Get the base component from ComponentDB
    auto it = components.find(componentKey);
    if (it != components.end()) {
        // Merge properties with the existing component
        luabridge::LuaRef& componentInstance = *(it->second->componentRef);
        SetComponentProperties(componentInstance, componentData);
    } else {
        std::shared_ptr<Component> newComponent = std::make_shared<Component>();

        // Create a new component instance
        std::shared_ptr<Component> baseComponent = ComponentDB::AddComponent(componentType);
        luabridge::LuaRef componentInstance = luabridge::newTable(ComponentManager::lua_state);
        ComponentDB::EstablishInheritance(componentInstance, *(baseComponent->componentRef));
        componentInstance["key"] = componentKey;

        // Set properties on the new component instance
        SetComponentProperties(componentInstance, componentData);

        if (!componentInstance["enabled"].isBool()) {
            componentInstance["enabled"] = true;
        }

        // Store the new component instance in the components map
        newComponent->componentRef = std::make_shared<luabridge::LuaRef>(componentInstance);

        newComponent->type = componentType;
        newComponent->key = componentKey;

        // If an actor is provided, set the actor for the component
        if (actor) {
            newComponent->actor = actor;
        }

        components[componentKey] = newComponent;
    }
}
void SetComponentProperties(luabridge::LuaRef& componentInstance, const rapidjson::Value& componentData) {
    for (auto& member : componentData.GetObject()) {
        std::string key = member.name.GetString();
        if (member.value.IsString()) {
            componentInstance[key] = member.value.GetString();
        } else if (member.value.IsInt()) {
            componentInstance[key] = member.value.GetInt();
        } else if (member.value.IsBool()) {
            componentInstance[key] = member.value.GetBool();
        } else if (member.value.IsFloat()) {
            componentInstance[key] = member.value.GetFloat();
        } else if (member.value.IsDouble()) {
            componentInstance[key] = member.value.GetDouble();
        }
    }
}

// Function to create a copy of an existing component
std::shared_ptr<Component> LoadExistingComponent(std::shared_ptr<Component> component) {
    // Get the base component from ComponentDB
    luabridge::LuaRef componentInstance = luabridge::newTable(ComponentManager::lua_state);
    ComponentDB::EstablishInheritance(componentInstance, *component->componentRef);
 
    std::shared_ptr<Component> componentPtr = std::make_shared<Component>();
    componentPtr->componentRef = std::make_shared<luabridge::LuaRef>(componentInstance);
    componentPtr->key = component->key;
    return componentPtr;
}

/*
    * Load an actor template 
    * This function will load an actor template from a file and store it in the templates hash table
*/
void Scene::LoadActorTemplate(const std::string& templateFp, const std::string& template_name) {
    rapidjson::Document doc;
    ReadJsonFile(templateFp, doc);
    const rapidjson::Value& actor_template = doc;

    std::shared_ptr<ActorTemplate> new_actor_template = std::make_shared<ActorTemplate>(template_name);

    if(actor_template.HasMember("name") && actor_template["name"].IsString()) {
        new_actor_template->name = actor_template["name"].GetString();
    }

    if(actor_template.HasMember("components")) {
        const rapidjson::Value& components = actor_template["components"];

        for (auto& component : components.GetObject()) {
            std::string componentKey = component.name.GetString();
            std::string componentType = component.value["type"].GetString();
            LoadComponent(componentKey, componentType, new_actor_template->components, component.value);
        }
    }

    templates[template_name] = std::move(new_actor_template);
}

/*
    Function that loops through the actors and calles the Update function, then the LateUpdate function.
*/
void Scene::Update() {
    // Call the OnStart for the new components.
    for(auto& component : onStartComponents) {
        if(component->IsEnabled()) {
            try {
                (*component->componentRef)["OnStart"](*component->componentRef);
            } catch (luabridge::LuaException const& e) {
                ReportError(component->actor->name, e);
            }
        }
    }
    onStartComponents.clear();

    for(auto& component : onUpdateComponents) {
        if(component->IsEnabled()) {
            try {
                (*component->componentRef)["OnUpdate"](*component->componentRef);
            } catch (luabridge::LuaException const& e) {
                ReportError(component->actor->name, e);
            }
        }
    }

    for(const auto& component : onLateUpdateComponents) {
        if(component->IsEnabled()) {
            try {
                (*component->componentRef)["OnLateUpdate"](*component->componentRef);
            } catch (luabridge::LuaException const& e) {
                ReportError(component->actor->name, e);
            }
        }
    }

    // Call OnDestroy
    for (const auto& component : onDestroyComponents) {
        try {
            if ((*component->componentRef)["OnDestroy"].isFunction()) {
                (*component->componentRef)["OnDestroy"](*component->componentRef);
            }
        } catch (luabridge::LuaException const& e) {
            ReportError(component->actor->name, e);
        }
    }
    onDestroyComponents.clear();

    // Add collected actors to the scene_actors vector
    for (const auto& newActor : new_actors_to_add) {
        scene_actors.push_back(newActor);
        // actor_name_map[newActor->name].push_back(newActor);
        for(auto& componentPair : newActor->components) {
            if(componentPair.second->hasStart && !componentPair.second->addedStart) {
                onStartComponents.push_back(componentPair.second);
                componentPair.second->addedStart = true;
            }
            if(componentPair.second->hasUpdate && !componentPair.second->addedUpdate) {
                onUpdateComponents.push_back(componentPair.second);
                componentPair.second->addedUpdate = true;
            }
            if(componentPair.second->hasLateUpdate && !componentPair.second->addedLate) {
                onLateUpdateComponents.push_back(componentPair.second);
                componentPair.second->addedLate = true;
            }
        }
    }
    new_actors_to_add.clear();

    // Add components from ComponentManager::components_to_add
    for (const auto& component : ComponentManager::components_to_add) {
        if (component->hasStart && !component->addedStart) {
            onStartComponents.push_back(component);
        }
        if (component->hasUpdate && !component->addedUpdate) {
            onUpdateComponents.push_back(component);
        }
        if (component->hasLateUpdate && !component->addedLate) {
            onLateUpdateComponents.push_back(component);
        }
    }
    ComponentManager::components_to_add.clear();
    
    // Remove components from ComponentManager::components_to_remove
    for (const auto& component : ComponentManager::components_to_remove) {
        auto removeFromList = [](std::vector<std::shared_ptr<Component>>& list, const std::shared_ptr<Component>& component) {
            list.erase(std::remove(list.begin(), list.end(), component), list.end());
        };

        if(component->hasDestroy || component->isRb) {
            try {
                (*component->componentRef)["OnDestroy"](*component->componentRef);
            } catch (luabridge::LuaException const& e) {
                ReportError(component->actor->name, e);
            }
        }

        removeFromList(onStartComponents, component);
        removeFromList(onUpdateComponents, component);
        removeFromList(onLateUpdateComponents, component);
    }
    ComponentManager::components_to_remove.clear();
    

    // Remove the actors - delete them.
    for(auto& actor : actors_to_remove) {
        for (auto& componentPair : actor->components) {
            auto component = componentPair.second;
            auto removeFromList = [](std::vector<std::shared_ptr<Component>>& list, const std::shared_ptr<Component>& component) {
                list.erase(std::remove(list.begin(), list.end(), component), list.end());
            };

            removeFromList(onStartComponents, component);
            removeFromList(onUpdateComponents, component);
            removeFromList(onLateUpdateComponents, component);
        }

        scene_actors.erase(find(scene_actors.begin(), scene_actors.end(), actor));
        auto it = actor_name_map.find(actor->name);
        if (it != actor_name_map.end()) {
            it->second.erase(std::remove(it->second.begin(), it->second.end(), actor), it->second.end());
            if (it->second.empty()) {
                actor_name_map.erase(it);
            }
        }
    }

    actors_to_remove.clear();
}

// Instantiate an actor based on a template name
luabridge::LuaRef Scene::InstantiateActor(const std::string& template_name) {
    // Want to add the actor template_name to the templates if it exists, and if it doesnt' exist already.
    TemplateToSpawn(template_name);

    // Find the actor template
    auto it = Scene::templates.find(template_name);

    if(it != Scene::templates.end()) {
        // Template exists
        // First, want to create a new actor
        std::shared_ptr<Actor> newActor = std::make_shared<Actor>(it->second->name, template_name);

        // Add the components to the actor object
        for (const auto& componentPair : it->second->components) {
            std::shared_ptr<Component> newComponent = LoadExistingComponent(componentPair.second);
            newActor->components[componentPair.first] = newComponent;
            newComponent->SetComponentProps();
            newActor->componentsByType[componentPair.second->type].push_back(newComponent);
            newComponent->actor = newActor;
            newComponent->type = componentPair.second->type;
        }

        // Inject convenience references
        for (auto& componentPair : newActor->components) {
            newActor->InjectConvenienceReferences(componentPair.second);
        }

        // Add the new actor to the vector
        Scene::new_actors_to_add.push_back(newActor);
        actor_name_map[newActor->name].push_back(newActor);

        // Return the Actor table reference
        return luabridge::LuaRef(ComponentManager::lua_state, newActor.get());
    }else {
        // Template doesn't exist
        std::cout << "error: template " << template_name << " does not exist";
        exit(0);
    }
}

// Remove the Actor from the scene
void Scene::RemoveActor(const luabridge::LuaRef& actor_ref) {
    // Have the actor name, and its uuid
    if (actor_ref.isUserdata()) {
        Actor& actor = actor_ref.cast<Actor&>();
    
        int id = actor.id;

        for(auto& actor : scene_actors) {
            if(actor->id == id) {
                for(auto component : actor->components) {
                    component.second->enabled = false;
                    (*component.second->componentRef)["enabled"] = false;
                    if(component.second->hasDestroy) {
                        onDestroyComponents.push_back(component.second);
                    }
                }
                actors_to_remove.insert(actor);
            }
        }

        for(auto& actor : new_actors_to_add) {
            if(actor->id == id) {
                for(auto component : actor->components) {
                    component.second->enabled = false;
                    (*component.second->componentRef)["enabled"] = false;
                    if(component.second->hasDestroy) {
                        onDestroyComponents.push_back(component.second);
                    }
                }
                actors_to_remove.insert(actor);
            }
        }

        // Remove the actor from the actor_name_map
        auto it = actor_name_map.find(actor.name);
        if (it != actor_name_map.end()) {
            it->second.erase(std::remove_if(it->second.begin(), it->second.end(),
                [id](const std::shared_ptr<Actor>& a) { return a->id == id; }), it->second.end());
            if (it->second.empty()) {
                actor_name_map.erase(it);
            }
        }
    }
}
 
void Scene::TemplateToSpawn(const std::string& template_name) {
    std::string templateFp = "resources/actor_templates/" + template_name + ".template";
    if(templates.find(template_name) == templates.end() && template_name != "") {
        if(std::filesystem::exists(templateFp)){
            LoadActorTemplate(templateFp, template_name);
        } else {
            std::cout << "error: template " << template_name << " is missing";
            exit(0);
        }
    }
}

void ReportError(std::string& actor_name, const luabridge::LuaException& e) {
    // Get the error message
    std::string error_msg = e.what();
    
    // Normalize fps across platforms
    std::replace(error_msg.begin(), error_msg.end(), '\\','/');
    
    // Display (with color codes)
    std::cout << "\033[31m" << actor_name << " : " << error_msg << "\033[0m" << std::endl;
}

void Scene::LoadNewScene(const std::string& new_scene) {
    scene_to_load = new_scene;
    if(scene_to_load != curr_name) {
        load_new_scene = true;
    }else {
        load_new_scene = false;
    }
}

std::string Scene::GetCurrent() {
    return curr_name;
}

void Scene::DontDestroy(const luabridge::LuaRef& actor_ref) {
    if (actor_ref.isUserdata()) {
        Actor& actor = actor_ref.cast<Actor&>();
    
        int id = actor.id;

        for(auto& actor : scene_actors) {
            if(actor->id == id) {
                actor->dontDestroy = false;
                actors_to_keep.insert(actor);
            }
        }

        for(auto& actor : new_actors_to_add) {
            if(actor->id == id) {
                actor->dontDestroy = false;
                actors_to_keep.insert(actor);
            }
        }
    }
}
