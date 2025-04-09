#ifndef INPUT_H
#define INPUT_H

#include "SDL2/SDL.h"
#include "glm/glm.hpp"
#include <unordered_map>
#include <vector>
#include <string>

enum INPUT_STATE {
  INPUT_STATE_UP,
  INPUT_STATE_JUST_BECAME_DOWN,
  INPUT_STATE_DOWN,
  INPUT_STATE_JUST_BECAME_UP
};

class Input {
public:
	static void Init();
	static void ProcessEvent(const SDL_Event& event);
	static void LateUpdate();

	static bool GetKey(SDL_Scancode keycode);
	static bool GetKeyDown(SDL_Scancode keycode);
	static bool GetKeyUp(SDL_Scancode keycode);

	static bool GetKeyByName(const std::string& keycode);
	static bool GetKeyDownByName(const std::string& keycode);
	static bool GetKeyUpByName(const std::string& keycode);

	static glm::vec2 GetMousePosition();
	static bool GetMouseButton(int button);
	static bool GetMouseButtonDown(int button);
	static bool GetMouseButtonUp(int button);
	static float GetMouseScrollDelta();

	static void HideCursor();
	static void ShowCursor();
private:
	static SDL_Scancode GetScancodeFromKeyName(const std::string& keyName);

	// Mouse Input Managers
	static inline std::unordered_map<SDL_Scancode, INPUT_STATE> keyboard_states;
	static inline std::vector<SDL_Scancode> just_became_down_scancodes;
	static inline std::vector<SDL_Scancode> just_became_up_scancodes;
	static std::unordered_map<std::string, SDL_Scancode> key_name_to_scancode;

	// Mouse Input Managers
	static inline std::unordered_map<int, INPUT_STATE> mouse_button_states;
	static inline std::vector<int> just_became_down_buttons;
	static inline std::vector<int> just_became_up_buttons;
	static glm::vec2 mouse_position;
	static float mouse_scroll_delta;
};

#endif