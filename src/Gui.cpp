#include "Gui.h"

#include <unordered_map>
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"

ImGuiIO* Gui::io = nullptr;

std::unordered_map<std::string, luabridge::LuaRef> getKeyValueMap(luabridge::LuaRef& table) {
  using namespace luabridge;
  std::unordered_map<std::string, LuaRef> result;
  if (table.isNil()) { return result; }

  auto L = table.state();
  push(L, table); // push table

  // Check if table has a metatable with __index
  if (lua_getmetatable(L, -1)) {  // pushes metatable
    lua_pushstring(L, "__index"); // push "__index"
    lua_rawget(L, -2);            // get metatable.__index, pops "__index"
    
    if (lua_istable(L, -1)) {
      // We found a metatable with __index table, recursively get its properties
      LuaRef indexTable = LuaRef::fromStack(L, -1);
      
      // Recursively get properties from the __index table
      auto inheritedProps = getKeyValueMap(indexTable);
      result.insert(inheritedProps.begin(), inheritedProps.end());
    }
    
    lua_pop(L, 2); // pop metatable.__index and metatable
  }
  
  // Now get direct properties (will override any inherited ones with the same name)
  push(L, table); // make sure table is at top of stack
  

  lua_pushnil(L);  // push nil, so lua_next removes it from stack and puts (k, v) on stack
  while (lua_next(L, -2) != 0) { // -2, because we have table at -1
    if (lua_isstring(L, -2)) { // only store stuff with string keys
      result.emplace(lua_tostring(L, -2), LuaRef::fromStack(L, -1));
    }
    lua_pop(L, 1); // remove value, keep key for lua_next
  }

  lua_pop(L, 1); // pop table
  return result;
}

void Gui::InitGui(SDL_Window* window, SDL_GLContext gl_context, ImGuiTheme theme) {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  // Configure ImGui
  io = &ImGui::GetIO();
  io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
  io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // Enable Docking

  // Apply the selected theme
  switch (theme) {
    case ImGuiTheme::Dark:
      ImGui::StyleColorsDark();
      break;
    case ImGuiTheme::Light:
      ImGui::StyleColorsLight();
      break;
    case ImGuiTheme::Classic:
      ImGui::StyleColorsClassic();
      break;
  }

  // Setup Platform/Renderer backends
  ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
  ImGui_ImplOpenGL3_Init("#version 330");
}

void Gui::Render() {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool Gui::ProcessEvent(const SDL_Event& event) {
  return ImGui_ImplSDL2_ProcessEvent(&event);
}

void Gui::NewFrame() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL2_NewFrame();
  ImGui::NewFrame();
}

void Gui::ShowDemoWindow(bool* open) {
  ImGui::ShowDemoWindow(open);
}

bool Gui::BeginWindow(const char* name, bool* open, ImGuiWindowFlags flags) {
  return ImGui::Begin(name, open, flags);
}

void Gui::EndWindow() {
  ImGui::End();
}

bool Gui::Button(const char* label, const ImVec2& size) {
  return ImGui::Button(label, size);
}

bool Gui::Checkbox(const char* label, bool* v) {
  return ImGui::Checkbox(label, v);
}

bool Gui::SliderFloat(const char* label, float* v, float min, float max) {
  return ImGui::SliderFloat(label, v, min, max);
}

bool Gui::ColorEdit3(const char* label, float col[3]) {
  return ImGui::ColorEdit3(label, col);
}

bool Gui::ColorEdit4(const char* label, float col[4]) {
  return ImGui::ColorEdit4(label, col);
}

// Start the Engine specific gui's
void Gui::SceneHierarchyWindow(Scene* current_scene) {
  static std::shared_ptr<Component> selectedComponent = nullptr;
  static bool showComponentPopup = false;
  static int activeActorId= -1;
  static bool showCreateCustomPopup = false;
  static bool showCreateActorPopup = false;
  static char customComponentName[128] = "";
  static char actorNameBuffer[128] = "";

  // If scene doesn't exist, want to break out of fn.
  if(!current_scene) return;

  ImVec2 displaySize = ImGui::GetIO().DisplaySize;
  float windowWidth = displaySize.x * 0.5f;
  float windowHeight = displaySize.y * 0.8f;
  float hierarchyWidth = windowWidth * 0.3f;
  
  // Position the window in the top center
  ImGui::SetNextWindowPos(ImVec2((displaySize.x - windowWidth) * 0.5f, displaySize.y * 0.05f), ImGuiCond_FirstUseEver);
  ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight), ImGuiCond_FirstUseEver);

  // Add a parent window that will contain the child windows
  ImGui::Begin("Scene Inspector", nullptr);

  // Left side: Scene hierarchy
  ImGui::BeginChild("SceneHierarchy", ImVec2(hierarchyWidth, -40), true);
  ImGui::Text("Scene: %s", Scene::GetCurrent().c_str());
  ImGui::Separator();

  if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered()) {
    ImGui::OpenPopup("SceneCtxMenu");
  }

  if(ImGui::BeginPopup("SceneCtxMenu")) {
    if(ImGui::MenuItem("Create Actor")) {
      showCreateActorPopup = true;
      actorNameBuffer[0] = '\0'; // Clear buffer
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }

  // Handle create actor popup
  if(showCreateActorPopup) {
    ImGui::OpenPopup("CreateActorPopup");
    showCreateActorPopup = false; // Reset flag
  }

  if(ImGui::BeginPopupModal("CreateActorPopup", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::Text("Enter actor name:");
    bool enterPressed = ImGui::InputText("##ActorName", actorNameBuffer, sizeof(actorNameBuffer), ImGuiInputTextFlags_EnterReturnsTrue);
    
    bool createClicked = ImGui::Button("Create", ImVec2(95, 30));
    ImGui::SameLine();
    
    // Cancel button
    if(ImGui::Button("Cancel", ImVec2(95, 30))) {
      ImGui::CloseCurrentPopup();
    }
    
    // Handle actor creation
    if(enterPressed || createClicked) {
      if(strlen(actorNameBuffer) > 0) {
        // Create a new actor with the given name
        Scene::InstantiateActor(actorNameBuffer);

        ImGui::CloseCurrentPopup();
      }
    }
  
    ImGui::EndPopup();
  }

  // Loop through all the actors in the scene
  for(const auto& actor : Scene::scene_actors) {
    // Create a tree node for each actor so you can allow dropdowns
    bool actorOpen = ImGui::TreeNode(actor->name.c_str());

    // Handle ctx menu for actors
    if(ImGui::IsItemClicked(1)) {
      ImGui::OpenPopup((std::string("ActorContextMenu_" + actor->GetID())).c_str());
    }

    if (ImGui::BeginPopup(("ActorContextMenu_" + actor->GetID()))) {
      if (ImGui::MenuItem("Add Component")) {
        showComponentPopup = true;
        activeActorId = actor->GetID();
      }
      
      if (ImGui::MenuItem("Delete Actor")) {
        Scene::RemoveActorById(actor->GetID());
      }
      
      ImGui::EndPopup();
    }

    if (actorOpen) {
      // Display components for this actor
      for (const auto& [key, component] : actor->components) {
        std::string label = component->type + " (" + key + ")";

        // Highlight the selected component
        bool isSelected = (selectedComponent == component);
        if (isSelected) {
          ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));
        }

        if (ImGui::Selectable(label.c_str(), isSelected)) {
          selectedComponent = component;
        }

        if (isSelected) {
          ImGui::PopStyleColor();
        }
      }
      ImGui::TreePop();
    }
  }

  ImGui::EndChild();

  if (showComponentPopup) {
    ImGui::OpenPopup("ComponentCreationPopup");
    showComponentPopup = false; // Reset flag after opening
  }
  
  // Handle the component creation popup
  if (ImGui::BeginPopupModal("ComponentCreationPopup", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
    // Find the target actor
    std::shared_ptr<Actor> targetActor = nullptr;
    for (auto& a : Scene::scene_actors) {
      if (a->GetID() == activeActorId) {
        targetActor = a;
        break;
      }
    }
    
    if (targetActor) {
      ImGui::Text("Creating Component For: %s", targetActor->name.c_str());
      
      // Component buttons...
      if (ImGui::Button("Light Component", ImVec2(200, 30))) {
        // Add light component...
        targetActor->AddComponent("Light");
        ImGui::CloseCurrentPopup();
      }
      
      if (ImGui::Button("Custom Component", ImVec2(200, 30))) {
        showCreateCustomPopup = true;
        customComponentName[0] = '\0';
        ImGui::CloseCurrentPopup();
      }
      
      if (ImGui::Button("Cancel", ImVec2(200, 30))) {
        ImGui::CloseCurrentPopup();
      }
    }
    
    ImGui::EndPopup();
  }

  if(showCreateCustomPopup) {
    ImGui::OpenPopup("CustomCreationPopup");
    showComponentPopup = false; // Reset flag after opening
  }

  if(ImGui::BeginPopupModal("CustomCreationPopup", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::Text("Enter component name:");
    bool enterPressed = ImGui::InputText("##ComponentName", customComponentName, sizeof(customComponentName), ImGuiInputTextFlags_EnterReturnsTrue);
    
    bool createClicked = ImGui::Button("Create", ImVec2(95, 30));
    ImGui::SameLine();


    if(enterPressed || createClicked) {
      if(strlen(customComponentName) > 0) {
        // Find the target actor using the activeActorId
        std::shared_ptr<Actor> targetActor = nullptr;
        for (auto& a : Scene::scene_actors) {
          if (a->GetID() == activeActorId) {
            targetActor = a;
            break;
          }
        }

        std::string componentName = std::string(customComponentName);
        targetActor->AddComponentFromGui(componentName);

        showCreateCustomPopup = false;
        ImGui::CloseCurrentPopup();
      }
    }

    // Cancel button
    if (ImGui::Button("Cancel", ImVec2(200, 30))) {
      showCreateCustomPopup = false;
      ImGui::CloseCurrentPopup();
    }

    ImGui::EndPopup();
  }

  ImGui::Separator();
  ImGui::SetCursorPosX((ImGui::GetWindowWidth() - 120) * 0.5f); // Center the button
  if (ImGui::Button("Save Scene", ImVec2(120, 30))) {
    std::string scenePath = "resources/scenes/" + Scene::GetCurrent() + ".scene";
    if(current_scene->SaveToFile(scenePath)) {
      ImGui::OpenPopup("SaveSuccessPopup");
    }else {
      ImGui::OpenPopup("SaveErrorPopup");
    }
  }

  // Success popup
  if (ImGui::BeginPopup("SaveSuccessPopup")) {
    ImGui::Text("Scene saved successfully!");
    if (ImGui::Button("OK")) {
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }

  // Error popup
  if (ImGui::BeginPopup("SaveErrorPopup")) {
    ImGui::Text("Error saving scene!");
    if (ImGui::Button("OK")) {
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }

  // Second window: Component Properties (positioned at top-right)
  ImGui::SetNextWindowPos(ImVec2(displaySize.x - 320, 20), ImGuiCond_FirstUseEver);
  ImGui::SetNextWindowSize(ImVec2(200, 400), ImGuiCond_FirstUseEver);
  
  if (ImGui::Begin("Component Properties", nullptr)) {
    if (selectedComponent) {
      ImGui::Text("Properties: %s", selectedComponent->type.c_str());
      ImGui::Separator();

      // Handle different component types
      if (selectedComponent->type == "Light" && selectedComponent->isLC) {
        // Cast to LightComponent to access specific properties
        auto lightComponent = std::static_pointer_cast<LightComponent>(selectedComponent);
        
        // Light type dropdown
        const char* lightTypes[] = { "Directional", "Point", "Spot" };
        int currentType = static_cast<int>(lightComponent->GetType());
        if (ImGui::Combo("Light Type", &currentType, lightTypes, IM_ARRAYSIZE(lightTypes))) {
          lightComponent->SetType(static_cast<LightType>(currentType));
        }
        
        // Color editor
        glm::vec3 color = lightComponent->GetColor();
        float colorArray[3] = { color.r, color.g, color.b };
        if (ImGui::ColorEdit3("Color", colorArray)) {
          lightComponent->SetColor(glm::vec3(colorArray[0], colorArray[1], colorArray[2]));
        }
        
        // Intensity slider
        float intensity = lightComponent->GetIntensity();
        if (ImGui::SliderFloat("Intensity", &intensity, 0.0f, 5.0f)) {
          lightComponent->SetIntensity(intensity);
        }
        
        // Position editor (for point and spot lights)
        if (currentType != 0) { // Not directional
          glm::vec3 position = lightComponent->GetPosition();
          float posArray[3] = { position.x, position.y, position.z };
          if (ImGui::DragFloat3("Position", posArray, 0.1f)) {
            lightComponent->SetPosition(glm::vec3(posArray[0], posArray[1], posArray[2]));
          }
        }
        
        // Direction editor (for directional and spot lights)
        if (currentType == 0 || currentType == 2) { // Directional or Spot
          glm::vec3 direction = lightComponent->GetDirection();
          float dirArray[3] = { direction.x, direction.y, direction.z };
          if (ImGui::DragFloat3("Direction", dirArray, 0.1f)) {
            lightComponent->SetDirection(glm::vec3(dirArray[0], dirArray[1], dirArray[2]));
          }
        }
        
        // Spot light properties
        if (currentType == 2) { // Spot light
          float innerCutoff = lightComponent->GetInnerCutoff();
          if (ImGui::SliderFloat("Inner Cutoff", &innerCutoff, 0.0f, 90.0f)) {
            lightComponent->SetInnerCutoff(innerCutoff);
          }
          
          float outerCutoff = lightComponent->GetOuterCutoff();
          if (ImGui::SliderFloat("Outer Cutoff", &outerCutoff, 0.0f, 90.0f)) {
            lightComponent->SetOuterCutoff(outerCutoff);
          }
        }
        
        // Attenuation factors (for point and spot lights)
        if (currentType != 0) { // Not directional
          float constant = lightComponent->GetConstant();
          if (ImGui::SliderFloat("Constant", &constant, 0.0f, 1.0f)) {
            lightComponent->SetConstant(constant);
          }
          
          float linear = lightComponent->GetLinear();
          if (ImGui::SliderFloat("Linear", &linear, 0.0f, 1.0f)) {
            lightComponent->SetLinear(linear);
          }
          
          float quadratic = lightComponent->GetQuadratic();
          if (ImGui::SliderFloat("Quadratic", &quadratic, 0.0f, 1.0f)) {
            lightComponent->SetQuadratic(quadratic);
          }
        }
      } 
      else {
        // Generic Lua component properties
        if (selectedComponent->componentRef) {
          ImGui::Text("Lua Component");
          // Get the component lua ref
          std::shared_ptr<luabridge::LuaRef> componentRef = selectedComponent->componentRef;

          // Make sure it's a table
          if(componentRef->isTable()) {
            for(auto& pair : getKeyValueMap(*componentRef)) {
              auto& key = pair.first;
              auto& value = pair.second;

              // Skip system/internal properties
              if(key == "actor" || key == "__self" || key == "self" || 
                key[0] == '_' || key == "key" || key == "type") {
                continue;
              }
              
              // Create a unique ID for each property control to avoid ID conflicts
              ImGui::PushID(key.c_str());
              
              // Handle different types
              if(value.isNumber()) {
                // Get fresh value directly from the component
                float numValue = static_cast<float>((*componentRef)[key.c_str()].cast<double>());
                if(ImGui::SliderFloat(key.c_str(), &numValue, -100.0f, 100.0f)) {
                  // Set value directly on the componentRef, not the temporary value reference
                  (*componentRef)[key.c_str()] = numValue;
                }
              }
              else if(value.isBool()) {
                // Get fresh value directly from the component
                bool boolValue = (*componentRef)[key.c_str()].cast<bool>();
                if(ImGui::Checkbox(key.c_str(), &boolValue)) {
                  // Set value directly on the componentRef
                  (*componentRef)[key.c_str()] = boolValue;
                }
              }
              else if(value.isString()) {
                // Display as text for now
                ImGui::Text("%s: %s", key.c_str(), (*componentRef)[key.c_str()].cast<std::string>().c_str());
              }
              else if(value.isUserdata()) {
                // Try to handle as vec3 if appropriate
                if(value["x"].isNumber() && value["y"].isNumber() && value["z"].isNumber()) {
                  // Get fresh vector values
                  float vec[3] = {
                    static_cast<float>((*componentRef)[key.c_str()]["x"].cast<double>()),
                    static_cast<float>((*componentRef)[key.c_str()]["y"].cast<double>()),
                    static_cast<float>((*componentRef)[key.c_str()]["z"].cast<double>())
                  };
                  
                  if(ImGui::DragFloat3(key.c_str(), vec, 0.1f)) {
                    // Update the vector directly
                    (*componentRef)[key.c_str()]["x"] = vec[0];
                    (*componentRef)[key.c_str()]["y"] = vec[1];
                    (*componentRef)[key.c_str()]["z"] = vec[2];
                  }
                }
                else {
                  ImGui::Text("%s: [userdata]", key.c_str());
                }
              }
              else if(value.isTable()) {
                ImGui::Text("%s: [table]", key.c_str());
              }
              
              ImGui::PopID();
            }
          }
        }
      }
    }else {
      ImGui::Text("Select a component to view its properties");
    }
  }

  ImGui::End();
  ImGui::End();
}

void Gui::Shutdown() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
}