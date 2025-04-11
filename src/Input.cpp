//
//  Input.cpp
//  game_engine
//
//  Created by Noah Dujovny on 2/12/25.
//

#include "Input.h"
#include <iostream>


// Define the static membe variables
std::unordered_map<std::string, SDL_Scancode> Input::key_name_to_scancode;
float Input::mouse_scroll_delta;
glm::vec2 Input::mouse_position;

void Input::Init() {
  keyboard_states.clear();
  just_became_down_scancodes.clear();
  just_became_up_scancodes.clear();

  mouse_button_states.clear();
  just_became_up_buttons.clear();
  just_became_down_buttons.clear();
  mouse_scroll_delta = 0.0f;
  mouse_position = glm::vec2(0.0f, 0.0f);

  key_name_to_scancode = {
    {"up", SDL_SCANCODE_UP},
    {"down", SDL_SCANCODE_DOWN},
    {"right", SDL_SCANCODE_RIGHT},
    {"left", SDL_SCANCODE_LEFT},
    {"escape", SDL_SCANCODE_ESCAPE},
    {"lshift", SDL_SCANCODE_LSHIFT},
    {"rshift", SDL_SCANCODE_RSHIFT},
    {"lctrl", SDL_SCANCODE_LCTRL},
    {"rctrl", SDL_SCANCODE_RCTRL},
    {"lalt", SDL_SCANCODE_LALT},
    {"ralt", SDL_SCANCODE_RALT},
    {"tab", SDL_SCANCODE_TAB},
    {"return", SDL_SCANCODE_RETURN},
    {"enter", SDL_SCANCODE_RETURN},
    {"backspace", SDL_SCANCODE_BACKSPACE},
    {"delete", SDL_SCANCODE_DELETE},
    {"insert", SDL_SCANCODE_INSERT},
    {"space", SDL_SCANCODE_SPACE},
    {"a", SDL_SCANCODE_A},
    {"b", SDL_SCANCODE_B},
    {"c", SDL_SCANCODE_C},
    {"d", SDL_SCANCODE_D},
    {"e", SDL_SCANCODE_E},
    {"f", SDL_SCANCODE_F},
    {"g", SDL_SCANCODE_G},
    {"h", SDL_SCANCODE_H},
    {"i", SDL_SCANCODE_I},
    {"j", SDL_SCANCODE_J},
    {"k", SDL_SCANCODE_K},
    {"l", SDL_SCANCODE_L},
    {"m", SDL_SCANCODE_M},
    {"n", SDL_SCANCODE_N},
    {"o", SDL_SCANCODE_O},
    {"p", SDL_SCANCODE_P},
    {"q", SDL_SCANCODE_Q},
    {"r", SDL_SCANCODE_R},
    {"s", SDL_SCANCODE_S},
    {"t", SDL_SCANCODE_T},
    {"u", SDL_SCANCODE_U},
    {"v", SDL_SCANCODE_V},
    {"w", SDL_SCANCODE_W},
    {"x", SDL_SCANCODE_X},
    {"y", SDL_SCANCODE_Y},
    {"z", SDL_SCANCODE_Z},
    {"0", SDL_SCANCODE_0},
    {"1", SDL_SCANCODE_1},
    {"2", SDL_SCANCODE_2},
    {"3", SDL_SCANCODE_3},
    {"4", SDL_SCANCODE_4},
    {"5", SDL_SCANCODE_5},
    {"6", SDL_SCANCODE_6},
    {"7", SDL_SCANCODE_7},
    {"8", SDL_SCANCODE_8},
    {"9", SDL_SCANCODE_9},
    {"/", SDL_SCANCODE_SLASH},
    {";", SDL_SCANCODE_SEMICOLON},
    {"=", SDL_SCANCODE_EQUALS},
    {"-", SDL_SCANCODE_MINUS},
    {".", SDL_SCANCODE_PERIOD},
    {",", SDL_SCANCODE_COMMA},
    {"[", SDL_SCANCODE_LEFTBRACKET},
    {"]", SDL_SCANCODE_RIGHTBRACKET},
    {"\\", SDL_SCANCODE_BACKSLASH},
    {"'", SDL_SCANCODE_APOSTROPHE}
  };
}

void Input::ProcessEvent(const SDL_Event& event) {
  if(event.type == SDL_KEYDOWN) {
    SDL_Scancode scancode = event.key.keysym.scancode;
    if(keyboard_states[scancode] == INPUT_STATE_UP || keyboard_states[scancode] == INPUT_STATE_JUST_BECAME_UP) {
      keyboard_states[scancode] = INPUT_STATE_JUST_BECAME_DOWN;
      just_became_down_scancodes.push_back(scancode);
    }
  }else if(event.type == SDL_KEYUP) {
    SDL_Scancode scancode = event.key.keysym.scancode;
    if (keyboard_states[scancode] == INPUT_STATE_DOWN || keyboard_states[scancode] == INPUT_STATE_JUST_BECAME_DOWN) {
      keyboard_states[scancode] = INPUT_STATE_JUST_BECAME_UP;
      just_became_up_scancodes.push_back(scancode);
    }
  }else if(event.type == SDL_MOUSEMOTION) {
    mouse_position.x = event.motion.x;
    mouse_position.y = event.motion.y;
  }else if(event.type == SDL_MOUSEBUTTONDOWN) {
    int button = event.button.button;
    if(mouse_button_states[button] == INPUT_STATE_UP || mouse_button_states[button] == INPUT_STATE_JUST_BECAME_UP) {
      mouse_button_states[button] = INPUT_STATE_JUST_BECAME_DOWN;
      just_became_down_buttons.push_back(button);
    }
  }else if(event.type == SDL_MOUSEBUTTONUP) {
    int button = event.button.button;
    if (mouse_button_states[button] == INPUT_STATE_DOWN || mouse_button_states[button] == INPUT_STATE_JUST_BECAME_DOWN) {
      mouse_button_states[button] = INPUT_STATE_JUST_BECAME_UP;
      just_became_up_buttons.push_back(button);
    }
  }else if(event.type == SDL_MOUSEWHEEL) {
    mouse_scroll_delta = event.wheel.preciseY;
  }
}

void Input::LateUpdate() {
  for(auto scancode : just_became_down_scancodes) {
    keyboard_states[scancode] = INPUT_STATE_DOWN;
  }
  just_became_down_scancodes.clear();

  for(auto scancode : just_became_up_scancodes) {
    keyboard_states[scancode] = INPUT_STATE_UP;
  }
  just_became_up_scancodes.clear();

  for(auto button : just_became_down_buttons) {
    mouse_button_states[button] = INPUT_STATE_DOWN;
  }
  just_became_down_buttons.clear();

  for(auto button : just_became_up_buttons) {
    mouse_button_states[button] = INPUT_STATE_UP;
  }
  just_became_up_buttons.clear();

  mouse_scroll_delta = 0.0f;
}

bool Input::GetKey(SDL_Scancode keycode) {
  return keyboard_states[keycode] == INPUT_STATE_DOWN || keyboard_states[keycode] == INPUT_STATE_JUST_BECAME_DOWN;
}

bool Input::GetKeyDown(SDL_Scancode keycode) {
  return keyboard_states[keycode] == INPUT_STATE_JUST_BECAME_DOWN;
}

bool Input::GetKeyUp(SDL_Scancode keycode) {
  return keyboard_states[keycode] == INPUT_STATE_JUST_BECAME_UP;
}

bool Input::GetKeyByName(const std::string& keycode) {
  SDL_Scancode scancode = GetScancodeFromKeyName(keycode);
  if(scancode == SDL_SCANCODE_UNKNOWN) {
    std::cout << "scancode " << keycode <<  " is unknown?" << std::endl;
    return false;
  }
  return GetKey(scancode);
}

bool Input::GetKeyDownByName(const std::string& keycode) {
  SDL_Scancode scancode = GetScancodeFromKeyName(keycode);
  if(scancode == SDL_SCANCODE_UNKNOWN) {
    return false;
  }
  return GetKeyDown(scancode);
}

bool Input::GetKeyUpByName(const std::string& keycode) {
  SDL_Scancode scancode = GetScancodeFromKeyName(keycode);
  if(scancode == SDL_SCANCODE_UNKNOWN) {
    return false;
  }
  return GetKeyUp(scancode);
}

// Mouse Input Functions
glm::vec2 Input::GetMousePosition() {
  return mouse_position;
}

bool Input::GetMouseButton(int button) {
  return mouse_button_states[button] == INPUT_STATE_DOWN || mouse_button_states[button] == INPUT_STATE_JUST_BECAME_DOWN;
}

bool Input::GetMouseButtonDown(int button) {
  return mouse_button_states[button] == INPUT_STATE_JUST_BECAME_DOWN;
}

bool Input::GetMouseButtonUp(int button) {
  return mouse_button_states[button] == INPUT_STATE_JUST_BECAME_UP;
}

float Input::GetMouseScrollDelta() {
  return mouse_scroll_delta;
}

void Input::HideCursor() {
  SDL_ShowCursor(SDL_DISABLE);
}

void Input::ShowCursor() {
  SDL_ShowCursor(SDL_ENABLE);
}

SDL_Scancode Input::GetScancodeFromKeyName(const std::string& keyName) {
  auto it = key_name_to_scancode.find(keyName);
  if (it != key_name_to_scancode.end()) {
    return it->second;
  }
  return SDL_SCANCODE_UNKNOWN;
}
