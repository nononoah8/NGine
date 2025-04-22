#include "Gui.h"

ImGuiIO* Gui::io = nullptr;

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

void Gui::Shutdown() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
}