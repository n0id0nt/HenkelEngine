#include "Input.h"
#include <imgui_impl_sdl2.h>
#include "Window.h"
#include "pugixml.hpp"
#include <opengl\openglHelper.h>

Input::Input() 
    : m_mouseButtonStates{ false, false, false }, m_mousePosition(0, 0), m_keysDown{}, m_keysPressed{}, m_keysReleased{}, m_windowSize{0u,0u}, m_windowResized(false), m_quit(false), m_bindings()
{
}

Input::~Input()
{
}

void Input::Update()
{
    // reset the keys pressed and keys released arrays
    m_keysPressed.clear();
    m_keysReleased.clear();

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        // ImGui events
        ImGui_ImplSDL2_ProcessEvent(&event); // Forward your event to backend

        switch (event.type) 
        {
        case SDL_QUIT:
            m_quit = true;
            break;
        case SDL_MOUSEMOTION:
            onMouseMove(event);
            break;
        case SDL_MOUSEBUTTONDOWN:
            onMouseButtonDown(event);
            break;
        case SDL_MOUSEBUTTONUP:
            onMouseButtonUp(event);
            break;
        case SDL_KEYDOWN:
            onKeyDown(event);
            break;
        case SDL_KEYUP:
            onKeyUp(event);
            break;
        case SDL_WINDOWEVENT:
            onWindowEvent(event);
            break;
        }

    }
}

glm::vec2 Input::GetArrowDir()
{
    int x = isKeyDown(SDLK_RIGHT) - isKeyDown(SDLK_LEFT);
    int y = isKeyDown(SDLK_UP) - isKeyDown(SDLK_DOWN);
    return glm::vec2(x, y);
}

glm::vec2* Input::getMousePosition()
{
    return &m_mousePosition;
}

bool Input::isKeyDown(SDL_Keycode key)
{
    return m_keysDown.count(key);
}

bool Input::isKeyJustPressed(SDL_Keycode key)
{
    return m_keysPressed.count(key);
}

bool Input::isKeyJustReleased(SDL_Keycode key)
{
    return m_keysReleased.count(key);
}

void Input::onKeyDown(SDL_Event& event)
{
    SDL_Keycode key = event.key.keysym.sym;
    m_keysDown.insert(key);
    m_keysPressed.insert(key);
}

void Input::onKeyUp(SDL_Event& event)
{
    SDL_Keycode key = event.key.keysym.sym;
    m_keysDown.erase(key);
    m_keysReleased.insert(key);
}

bool Input::Quit() const
{
    return m_quit;
}

void Input::onWindowEvent(SDL_Event& event)
{
    switch (event.window.event)
    {
    case SDL_WINDOWEVENT_SIZE_CHANGED:
        m_windowSize = { event.window.data1, event.window.data2 };
        m_windowResized = true;
        break;
    }
}

bool Input::WasWindowResized() const
{
    return m_windowResized;
}

void Input::ClearWindowResizedFlag()
{
    m_windowResized = false;
}

glm::vec2 Input::WindowSize() const
{
    return m_windowSize;
}

void Input::LoadInputBindings(const std::string& file)
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(file.c_str());
    ASSERT(result);
}

void Input::CreateBinding(const std::string& input, SDL_Keycode code)
{
    auto it = m_bindings.find(input);
    if (it == m_bindings.end())
        m_bindings[input] = std::set{ code };
    else
        m_bindings[input].insert(code);
}

bool Input::isInputDown(const std::string& input)
{
    auto& keys = m_bindings[input];
    for (auto it = keys.begin(); it != keys.end(); ++it)
    {
        if (isKeyDown(*it))
            return true;
    }
    return false;
}

bool Input::isInputJustPressed(const std::string& input)
{
    auto& keys = m_bindings[input];
    for (auto it = keys.begin(); it != keys.end(); ++it)
    {
        if (isKeyJustPressed(*it))
            return true;
    }
    return false;
}

bool Input::isInputJustReleased(const std::string& input)
{
    auto& keys = m_bindings[input];
    for (auto it = keys.begin(); it != keys.end(); ++it)
    {
        if (isKeyJustReleased(*it))
            return true;
    }
    return false;
}

void Input::LUABind(sol::state& lua)
{
    lua.new_usertype<Input>("Input",
        "isInputDown", &Input::isInputDown,
        "getArrowDir", &Input::GetArrowDir
    );
    lua.set("Input", this);
}

void Input::onMouseButtonDown(SDL_Event& event)
{
    if (event.button.button == SDL_BUTTON_LEFT)
    {
        m_mouseButtonStates[Left] = true;
    }
    if (event.button.button == SDL_BUTTON_MIDDLE)
    {
        m_mouseButtonStates[Middle] = true;
    }
    if (event.button.button == SDL_BUTTON_RIGHT)
    {
        m_mouseButtonStates[Right] = true;
    }
}

void Input::onMouseMove(SDL_Event& event)
{
    m_mousePosition.x = event.motion.x;
    m_mousePosition.y = event.motion.y;
}

void Input::onMouseButtonUp(SDL_Event& event)
{
    if (event.button.button == SDL_BUTTON_LEFT)
    {
        m_mouseButtonStates[Left] = false;
    }
    if (event.button.button == SDL_BUTTON_MIDDLE)
    {
        m_mouseButtonStates[Middle] = false;
    }
    if (event.button.button == SDL_BUTTON_RIGHT)
    {
        m_mouseButtonStates[Right] = false;
    }
}

bool Input::getMouseButtonState(int buttonNumber)
{
    return m_mouseButtonStates[buttonNumber];
}