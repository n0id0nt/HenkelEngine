#pragma once
#include <SDL.h>
#include <glm\glm.hpp>
#include <vector>
#include <set>
#include "Event.h"
#include <tuple>
#include <unordered_map>
#include <string>
#include "LUABindable.h"

class Window;

typedef std::set<SDL_Keycode> Binding;

struct AxisBinding
{
	Binding positive;
	Binding negetive;
};

enum Axis
{
	positive,
	negetive
};

struct Axis2Binding
{
	Binding up;
	Binding down;
	Binding left;
	Binding right;
};

enum Axis2
{
	up,
	down,
	left,
	right
};

class Input : public LUABindable
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

	void LoadInputBindings(const std::string& fileDir, const std::string& levelFile);

	/// Handling Mouse Events
	void onMouseButtonDown(SDL_Event& event);
	void onMouseMove(SDL_Event& event);
	void onMouseButtonUp(SDL_Event& event);
	bool getMouseButtonState(int buttonNumber);
	glm::vec2* getMousePosition();

	///Handling Keyboard Events
	void onKeyDown(SDL_Event& event);
	void onKeyUp(SDL_Event& event);

	bool isKeyDown(SDL_Keycode key);
	bool isKeyJustPressed(SDL_Keycode key);
	bool isKeyJustReleased(SDL_Keycode key);

	bool isStringKeyDown(const std::string& input);
	bool isStringKeyJustPressed(const std::string& input);
	bool isStringKeyJustReleased(const std::string& input);

	//Handling Window Events
	bool Quit() const;
	void onWindowEvent(SDL_Event& event);

	bool WasWindowResized() const;
	void ClearWindowResizedFlag();
	glm::vec2 WindowSize() const;

	void LoadInputBindings(const std::string& file);
	void CreateButtonBinding(const std::string& input, SDL_Keycode code);
	void CreateVairableBinding(const std::string& input, SDL_Keycode code);
	void CreateAxisBinding(const std::string& input, SDL_Keycode code, Axis axis);
	void CreateAxis2Binding(const std::string& input, SDL_Keycode code, Axis2 axis);
	
	///Handling Binded Events
	bool isInputDown(const std::string& input);
	bool isInputJustPressed(const std::string& input);
	bool isInputJustReleased(const std::string& input);
	float getInputVariable(const std::string& input);
	float getInputAxis(const std::string& input);
	glm::vec2 getInputAxis2(const std::string& input);

	SDL_Keycode GetKeycodeFromString(const std::string& input);

	void LUABind(sol::state& lua) override;

private:

	std::vector<bool> m_mouseButtonStates;
	glm::vec2 m_mousePosition;
	std::set<SDL_Keycode> m_keysDown;
	std::set<SDL_Keycode> m_keysPressed;
	std::set<SDL_Keycode> m_keysReleased;
	bool m_quit;
	glm::vec2 m_windowSize;
	bool m_windowResized;

	std::unordered_map<std::string, Binding> m_buttonBindings;
	std::unordered_map<std::string, Binding> m_variableBindings;
	std::unordered_map<std::string, AxisBinding> m_axisBindings;
	std::unordered_map<std::string, Axis2Binding> m_axis2Bindings;
	static std::unordered_map<std::string, SDL_Keycode> s_keycodeMap;
};

