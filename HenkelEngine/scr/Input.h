#pragma once
#include <SDL.h>
#include <glm\glm.hpp>
#include <vector>
#include <set>
#include "Event.h"
#include <tuple>
#include <unordered_map>
#include <string>
#include <array>
#include "LUABindable.h"

class Window;

class Input : public LUABindable
{
private:
	struct Binding
	{
		std::set<SDL_Keycode> keycodes;
		std::set<SDL_GameControllerButton> controllerButtons;
	};

	struct AxisBinding
	{
		Binding positive;
		Binding negetive;
		std::set<SDL_GameControllerAxis> controllerAxis;
	};

	enum AxisComponent
	{
		positive,
		negetive
	};

	struct Axis2Binding
	{
		AxisBinding xAxis;
		AxisBinding yAxis;
	};

	enum Axis2Component
	{
		up,
		down,
		left,
		right
	};

	enum Axis2Direction
	{
		x,
		y
	};

	enum MouseButtons
	{
		Left,
		Middle,
		Right
	};

public:

	Input();
	~Input();

	void Update();

	void LoadInputBindings(const std::string& fileDir, const std::string& levelFile);

	void onMouseButtonDown(SDL_Event& event);
	void onMouseMove(SDL_Event& event);
	void onMouseButtonUp(SDL_Event& event);
	bool getMouseButtonState(int buttonNumber);
	glm::vec2* getMousePosition();

	void onKeyDown(SDL_Event& event);
	void onKeyUp(SDL_Event& event);
	
	void onControllerButtonDown(SDL_Event& event);
	void onControllerButtonUp(SDL_Event& event);

	bool isKeyDown(SDL_Keycode key);
	bool isKeyJustPressed(SDL_Keycode key);
	bool isKeyJustReleased(SDL_Keycode key);
	
	bool isControllerButtonDown(SDL_GameControllerButton button);
	bool isControllerButtonJustPressed(SDL_GameControllerButton button);
	bool isControllerButtonJustReleased(SDL_GameControllerButton button);

	float getControllerAxisValue(SDL_GameControllerAxis code);

	bool isStringKeyDown(const std::string& input);
	bool isStringKeyJustPressed(const std::string& input);
	bool isStringKeyJustReleased(const std::string& input);
	
	bool isStringControllerButtonDown(const std::string& input);
	bool isStringControllerButtonJustPressed(const std::string& input);
	bool isStringControllerButtonJustReleased(const std::string& input);

	float getStringControllerAxisValue(const std::string& input);

	bool Quit() const;
	void onWindowEvent(SDL_Event& event);

	bool WasWindowResized() const;
	void ClearWindowResizedFlag();
	glm::vec2 WindowSize() const;

	void LoadInputBindings(const std::string& file);

	void CreateButtonBinding(const std::string& input, SDL_Keycode code);
	void CreateButtonBinding(const std::string& input, SDL_GameControllerButton code);

	void CreateVairableBinding(const std::string& input, SDL_Keycode code);
	void CreateVairableBinding(const std::string& input, SDL_GameControllerButton code);
	void CreateVairableBinding(const std::string& input, SDL_GameControllerAxis code);

	void CreateAxisBinding(const std::string& input, SDL_Keycode code, AxisComponent axis);
	void CreateAxisBinding(const std::string& input, SDL_GameControllerButton code, AxisComponent axis);
	void CreateAxisBinding(const std::string& input, SDL_GameControllerAxis code);

	void CreateAxis2Binding(const std::string& input, SDL_Keycode code, Axis2Component axis);
	void CreateAxis2Binding(const std::string& input, SDL_GameControllerButton code, Axis2Component axis);
	void CreateAxis2Binding(const std::string& input, SDL_GameControllerAxis code, Axis2Direction axis);
	
	bool isInputDown(const std::string& input);
	bool isInputJustPressed(const std::string& input);
	bool isInputJustReleased(const std::string& input);
	float getInputVariable(const std::string& input);
	float getInputAxis(const std::string& input);
	glm::vec2 getInputAxis2(const std::string& input);

	SDL_Keycode GetKeycodeFromString(const std::string& input);
	SDL_GameControllerButton GetControllerButtonFromString(const std::string& input);
	SDL_GameControllerAxis GetControllerAxisFromString(const std::string& input);

	void LUABind(sol::state& lua) override;

private:

	std::vector<bool> m_mouseButtonStates;
	glm::vec2 m_mousePosition;

	std::set<SDL_Keycode> m_keysDown;
	std::set<SDL_Keycode> m_keysPressed;
	std::set<SDL_Keycode> m_keysReleased;	

	std::set<SDL_GameControllerButton> m_controllerButtonsDown;
	std::set<SDL_GameControllerButton> m_controllerButtonsPressed;
	std::set<SDL_GameControllerButton> m_controllerButtonsReleased;

	bool m_quit;
	glm::vec2 m_windowSize;
	bool m_windowResized;
	SDL_Joystick* m_joystick;

	SDL_GameController* m_gameController;

	std::unordered_map<std::string, Binding> m_buttonBindings;
	std::unordered_map<std::string, Binding> m_variableBindings;
	std::unordered_map<std::string, AxisBinding> m_axisBindings;
	std::unordered_map<std::string, Axis2Binding> m_axis2Bindings;
	static std::unordered_map<std::string, SDL_Keycode> s_keycodeMap;
	static std::unordered_map<std::string, SDL_GameControllerButton> s_controllerButtonMap;
	static std::unordered_map<std::string, SDL_GameControllerAxis> s_controllerAxisMap;

	float m_deadzone;

};

