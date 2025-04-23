#ifndef GUI_H
#define GUI_H

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_sdl2.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <SDL2/SDL.h>

#include "SceneDB.hpp"
#include "Actor.hpp"
#include "ComponentDB.hpp"
#include "Component.h"

enum class ImGuiTheme {
  Dark,
  Light,
  Classic
};

class Gui {
public:
  static void InitGui(SDL_Window* window, SDL_GLContext gl_context, ImGuiTheme theme = ImGuiTheme::Dark);
  static void Render();
  static void Shutdown();
  static bool ProcessEvent(const SDL_Event& event);

  // Lifecycle function(s)
  static void NewFrame();

  // Demo and debugging
  static void ShowDemoWindow(bool* open = nullptr);

  // UI helpers
  static bool BeginWindow(const char* name, bool* open = nullptr, ImGuiWindowFlags flags = 0);
  static void EndWindow();
  static bool Button(const char* label, const ImVec2& size = ImVec2(0, 0));
  static bool Checkbox(const char* label, bool* v);
  static bool SliderFloat(const char* label, float* v, float min, float max);
  static bool ColorEdit3(const char* label, float col[3]);
  static bool ColorEdit4(const char* label, float col[4]);
  
  // Start the Scene Hierarchy stuff
  static void SceneHierarchyWindow(Scene* current_scene);

  // Save the new Scene Hierarchy w new components and properties
  static void SaveHierarchy();
private:
  static ImGuiIO* io;
};


#endif