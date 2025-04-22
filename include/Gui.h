#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_sdl2.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <SDL2/SDL.h>

enum class ImGuiTheme {
  Dark,
  Light,
  Classic
};

class Gui {
public:
  static void InitGui(SDL_Window* window, SDL_GLContext gl_context, ImGuiTheme theme = ImGuiTheme::Dark);
  static void Shutdown();
private:
  static ImGuiIO* io;
};