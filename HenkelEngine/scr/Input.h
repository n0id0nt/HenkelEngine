#pragma once
#include <SDL.h>
#include <glm\glm.hpp>
#include <vector>
#include <set>
#include "Event.h"
#include <tuple>

class Window;

class Input
{
public:
	enum MouseButtons
	{
		Left,
		Middle,
		Right
	};

	static Window* window;

	static void Update();

	static glm::vec2 GetArrowDir();

	/// Handling Mouse Event
	static void onMouseButtonDown(SDL_Event& event);
	static void onMouseMove(SDL_Event& event);
	static void onMouseButtonUp(SDL_Event& event);
	static bool getMouseButtonState(int buttonNumber);
	static glm::vec2* getMousePosition();

	///Handling Keyboard Event
	static void onKeyDown(SDL_Event& event);
	static void onKeyUp(SDL_Event& event);
	static bool isKeyDown(SDL_Keycode key);
	static bool isKeyJustPressed(SDL_Keycode key);
	static bool isKeyJustReleased(SDL_Keycode key);

	//Handling Window Event
	static bool Quit();
	static void onWindowEvent(SDL_Event& event);

private:

	static std::vector<bool> m_mouseButtonStates;
	static glm::vec2 m_mousePosition;
	static std::set<SDL_Keycode> m_keysDown;
	static std::set<SDL_Keycode> m_keysPressed;
	static std::set<SDL_Keycode> m_keysReleased;
	static bool m_quit;
};

