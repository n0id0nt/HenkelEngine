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

	Input();
	~Input();

	void Update();

	glm::vec2 GetArrowDir();

	/// Handling Mouse Event
	void onMouseButtonDown(SDL_Event& event);
	void onMouseMove(SDL_Event& event);
	void onMouseButtonUp(SDL_Event& event);
	bool getMouseButtonState(int buttonNumber);
	glm::vec2* getMousePosition();

	///Handling Keyboard Event
	void onKeyDown(SDL_Event& event);
	void onKeyUp(SDL_Event& event);
	bool isKeyDown(SDL_Keycode key);
	bool isKeyJustPressed(SDL_Keycode key);
	bool isKeyJustReleased(SDL_Keycode key);

	//Handling Window Event
	bool Quit() const;
	void onWindowEvent(SDL_Event& event);

	bool WasWindowResized() const;
	void ClearWindowResizedFlag();
	glm::vec2 WindowSize() const;

private:

	std::vector<bool> m_mouseButtonStates;
	glm::vec2 m_mousePosition;
	std::set<SDL_Keycode> m_keysDown;
	std::set<SDL_Keycode> m_keysPressed;
	std::set<SDL_Keycode> m_keysReleased;
	bool m_quit;
	glm::vec2 m_windowSize;
	bool m_windowResized;
};

