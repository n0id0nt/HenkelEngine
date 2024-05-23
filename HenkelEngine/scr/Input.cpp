#include "Input.h"
#include <imgui_impl_sdl2.h>
#include "Window.h"
#include "pugixml.hpp"
#include <opengl\openglHelper.h>
#include <iostream>
#include <format>

std::unordered_map<std::string, SDL_Keycode> Input::s_keycodeMap = {
    {"return",              SDLK_RETURN},
    {"escape",              SDLK_ESCAPE},
    {"backspace",           SDLK_BACKSPACE},
    {"tab",                 SDLK_TAB},
    {"space",               SDLK_SPACE},
    {"exclaim",             SDLK_EXCLAIM},
    {"quotedbl",            SDLK_QUOTEDBL},
    {"hash",                SDLK_HASH},
    {"percent",             SDLK_PERCENT},
    {"dollar",              SDLK_DOLLAR},
    {"ampersand",           SDLK_AMPERSAND},
    {"quotesingle",         SDLK_QUOTE},
    {"leftparen",           SDLK_LEFTPAREN},
    {"rightparen",          SDLK_RIGHTPAREN},
    {"asterisk",            SDLK_ASTERISK},
    {"plus",                SDLK_PLUS},
    {"comma",               SDLK_COMMA},
    {"minus",               SDLK_MINUS},
    {"period",              SDLK_PERIOD},
    {"slash",               SDLK_SLASH},
    {"0",                   SDLK_0},
    {"1",                   SDLK_1},
    {"2",                   SDLK_2},
    {"3",                   SDLK_3},
    {"4",                   SDLK_4},
    {"5",                   SDLK_5},
    {"6",                   SDLK_6},
    {"7",                   SDLK_7},
    {"8",                   SDLK_8},
    {"9",                   SDLK_9},
    {"colon",               SDLK_COLON},
    {"semicolon",           SDLK_SEMICOLON},
    {"less",                SDLK_LESS},
    {"equals",              SDLK_EQUALS},
    {"greater",             SDLK_GREATER},
    {"question",            SDLK_QUESTION},
    {"at",                  SDLK_AT},
    {"leftbracket",         SDLK_LEFTBRACKET},
    {"backslash",           SDLK_BACKSLASH},
    {"rightbracket",        SDLK_RIGHTBRACKET},
    {"caret",               SDLK_CARET},
    {"underscore",          SDLK_UNDERSCORE},
    {"backquote",           SDLK_BACKQUOTE},
    {"a",                   SDLK_a},
    {"b",                   SDLK_b},
    {"c",                   SDLK_c},
    {"d",                   SDLK_d},
    {"e",                   SDLK_e},
    {"f",                   SDLK_f},
    {"g",                   SDLK_g},
    {"h",                   SDLK_h},
    {"i",                   SDLK_i},
    {"j",                   SDLK_j},
    {"k",                   SDLK_k},
    {"l",                   SDLK_l},
    {"m",                   SDLK_m},
    {"n",                   SDLK_n},
    {"o",                   SDLK_o},
    {"p",                   SDLK_p},
    {"q",                   SDLK_q},
    {"r",                   SDLK_r},
    {"s",                   SDLK_s},
    {"t",                   SDLK_t},
    {"u",                   SDLK_u},
    {"v",                   SDLK_v},
    {"w",                   SDLK_w},
    {"x",                   SDLK_x},
    {"y",                   SDLK_y},
    {"z",                   SDLK_z},
    {"capslock",            SDLK_CAPSLOCK},
    {"f1",                  SDLK_F1},
    {"f2",                  SDLK_F2},
    {"f3",                  SDLK_F3},
    {"f4",                  SDLK_F4},
    {"f5",                  SDLK_F5},
    {"f6",                  SDLK_F6},
    {"f7",                  SDLK_F7},
    {"f8",                  SDLK_F8},
    {"f9",                  SDLK_F9},
    {"f10",                 SDLK_F10},
    {"f11",                 SDLK_F11},
    {"f12",                 SDLK_F12},
    {"printscreen",         SDLK_PRINTSCREEN},
    {"scrolllock",          SDLK_SCROLLLOCK},
    {"pause",               SDLK_PAUSE},
    {"insert",              SDLK_INSERT},
    {"home",                SDLK_HOME},
    {"pageup",              SDLK_PAGEUP},
    {"delete",              SDLK_DELETE},
    {"end",                 SDLK_END},
    {"pagedown",            SDLK_PAGEDOWN},
    {"right",               SDLK_RIGHT},
    {"left",                SDLK_LEFT},
    {"down",                SDLK_DOWN},
    {"up",                  SDLK_UP},
    {"numberlockclear",     SDLK_NUMLOCKCLEAR},
    {"kpdivide",            SDLK_KP_DIVIDE},
    {"kpmultiply",          SDLK_KP_MULTIPLY},
    {"kpminus",             SDLK_KP_MINUS},
    {"kpplus",              SDLK_KP_PLUS},
    {"kpequals",            SDLK_KP_EQUALS},
    {"kpenter",             SDLK_KP_ENTER},
    {"kp1",                 SDLK_KP_1},
    {"kp2",                 SDLK_KP_2},
    {"kp3",                 SDLK_KP_3},
    {"kp4",                 SDLK_KP_4},
    {"kp5",                 SDLK_KP_5},
    {"kp6",                 SDLK_KP_6},
    {"kp7",                 SDLK_KP_7},
    {"kp8",                 SDLK_KP_8},
    {"kp9",                 SDLK_KP_9},
    {"kp0",                 SDLK_KP_0},
    {"kpperiod",            SDLK_KP_PERIOD},
    {"kpcomma",             SDLK_KP_COMMA},
    {"kpequals400",         SDLK_KP_EQUALSAS400},
    {"application",         SDLK_APPLICATION},
    {"power",               SDLK_POWER},
    {"f13",                 SDLK_F13},
    {"f14",                 SDLK_F14},
    {"f15",                 SDLK_F15},
    {"f16",                 SDLK_F16},
    {"f17",                 SDLK_F17},
    {"f18",                 SDLK_F18},
    {"f19",                 SDLK_F19},
    {"f20",                 SDLK_F20},
    {"f21",                 SDLK_F21},
    {"f22",                 SDLK_F22},
    {"f23",                 SDLK_F23},
    {"f24",                 SDLK_F24},
    {"execute",             SDLK_EXECUTE},
    {"hemp",                SDLK_HELP},
    {"menu",                SDLK_MENU},
    {"select",              SDLK_SELECT},
    {"stop",                SDLK_STOP},
    {"again",               SDLK_AGAIN},
    {"undo",                SDLK_UNDO},
    {"cut",                 SDLK_CUT},
    {"copy",                SDLK_COPY},
    {"paste",               SDLK_PASTE},
    {"find",                SDLK_FIND},
    {"mute",                SDLK_MUTE},
    {"volumeup",            SDLK_VOLUMEUP},
    {"volumedown",          SDLK_VOLUMEDOWN},
    {"alterase",            SDLK_ALTERASE},
    {"sysreq",              SDLK_SYSREQ},
    {"cancel",              SDLK_CANCEL},
    {"clear",               SDLK_CLEAR},
    {"prior",               SDLK_PRIOR},
    {"return2",             SDLK_RETURN2},
    {"seperator",           SDLK_SEPARATOR},
    {"out",                 SDLK_OUT},
    {"oper",                SDLK_OPER},
    {"clearagain",          SDLK_CLEARAGAIN},
    {"crsel",               SDLK_CRSEL},
    {"exsel",               SDLK_EXSEL},
    {"kp00",                SDLK_KP_00},
    {"kp000",               SDLK_KP_000},
    {"thousandsseparator",  SDLK_THOUSANDSSEPARATOR},
    {"decimalseperator",    SDLK_DECIMALSEPARATOR},
    {"currencyunit",        SDLK_CURRENCYUNIT},
    {"currencysubunit",     SDLK_CURRENCYSUBUNIT},
    {"kpleftparen",         SDLK_KP_LEFTPAREN},
    {"kprightparen",        SDLK_KP_RIGHTPAREN},
    {"kpleftbrace",         SDLK_KP_LEFTBRACE},
    {"kprightbreace",       SDLK_KP_RIGHTBRACE},
    {"kptab",               SDLK_KP_TAB},
    {"kpbackspace",         SDLK_KP_BACKSPACE},
    {"kpa",                 SDLK_KP_A},
    {"kpb",                 SDLK_KP_B},
    {"kpc",                 SDLK_KP_C},
    {"kpd",                 SDLK_KP_D},
    {"kpe",                 SDLK_KP_E},
    {"kpf",                 SDLK_KP_F},
    {"kpxor",               SDLK_KP_XOR},
    {"kppower",             SDLK_KP_POWER},
    {"kppercent",           SDLK_KP_PERCENT},
    {"kpless",              SDLK_KP_LESS},
    {"kpgreater",           SDLK_KP_GREATER},
    {"kpampersand",         SDLK_KP_AMPERSAND},
    {"kpdblampersand",      SDLK_KP_DBLAMPERSAND},
    {"kpverticalbar",       SDLK_KP_VERTICALBAR},
    {"kpdblverticalbar",    SDLK_KP_DBLVERTICALBAR},
    {"kpcolon",             SDLK_KP_COLON},
    {"kphash",              SDLK_KP_HASH},
    {"kpspace",             SDLK_KP_SPACE},
    {"kpat",                SDLK_KP_AT},
    {"kpexclam",            SDLK_KP_EXCLAM},
    {"kpmemstore",          SDLK_KP_MEMSTORE},
    {"kpmemrecall",         SDLK_KP_MEMRECALL},
    {"kpmemclear",          SDLK_KP_MEMCLEAR},
    {"kpmemadd",            SDLK_KP_MEMADD},
    {"kpmemsubtract",       SDLK_KP_MEMSUBTRACT},
    {"kpmemultiply",        SDLK_KP_MEMMULTIPLY},
    {"kpmemdivide",         SDLK_KP_MEMDIVIDE},
    {"kpplusminus",         SDLK_KP_PLUSMINUS},
    {"kpclear",             SDLK_KP_CLEAR},
    {"kpclearentry",        SDLK_KP_CLEARENTRY},
    {"kpbinary",            SDLK_KP_BINARY},
    {"kpoctal",             SDLK_KP_OCTAL},
    {"kpdecimal",           SDLK_KP_DECIMAL},
    {"kphexadecimal",       SDLK_KP_HEXADECIMAL},
    {"lctrl",               SDLK_LCTRL},
    {"lshift",              SDLK_LSHIFT},
    {"lalt",                SDLK_LALT},
    {"lgui",                SDLK_LGUI},
    {"rctrl",               SDLK_RCTRL},
    {"rshift",              SDLK_RSHIFT},
    {"ralt",                SDLK_RALT},
    {"rgui",                SDLK_RGUI},
    {"mode",                SDLK_MODE},
    {"audionext",           SDLK_AUDIONEXT},
    {"audioprev",           SDLK_AUDIOPREV},
    {"audiostop",           SDLK_AUDIOSTOP},
    {"audioplay",           SDLK_AUDIOPLAY},
    {"audiomute",           SDLK_AUDIOMUTE},
    {"mediaselect",         SDLK_MEDIASELECT},
    {"www",                 SDLK_WWW},
    {"mail",                SDLK_MAIL},
    {"calculator",          SDLK_CALCULATOR},
    {"computer",            SDLK_COMPUTER},
    {"acsearch",            SDLK_AC_SEARCH},
    {"achome",              SDLK_AC_HOME},
    {"acback",              SDLK_AC_BACK},
    {"acforward",           SDLK_AC_FORWARD},
    {"acstop",              SDLK_AC_STOP},
    {"acrefresh",           SDLK_AC_REFRESH},
    {"acbookmarks",         SDLK_AC_BOOKMARKS},
    {"kpbrightnessdown",    SDLK_BRIGHTNESSDOWN},
    {"kpbrightnessup",      SDLK_BRIGHTNESSUP},
    {"kpdisplayswitch",     SDLK_DISPLAYSWITCH},
    {"kbdillumtoggle",      SDLK_KBDILLUMTOGGLE},
    {"kbdillumdown",        SDLK_KBDILLUMDOWN},
    {"kbdillumup",          SDLK_KBDILLUMUP},
    {"eject",               SDLK_EJECT},
    {"sleep",               SDLK_SLEEP},
    {"app1",                SDLK_APP1},
    {"app2",                SDLK_APP2},
    {"audiorewind",         SDLK_AUDIOREWIND},
    {"audiofastforward",    SDLK_AUDIOFASTFORWARD},
};

std::unordered_map<std::string, SDL_GameControllerButton> Input::s_controllerButtonMap = {
    {"a",           SDL_CONTROLLER_BUTTON_A},
    {"b",           SDL_CONTROLLER_BUTTON_B},
    {"x",           SDL_CONTROLLER_BUTTON_X},
    {"y",           SDL_CONTROLLER_BUTTON_Y},
    {"select",      SDL_CONTROLLER_BUTTON_BACK},
    {"guide",       SDL_CONTROLLER_BUTTON_GUIDE},
    {"start",       SDL_CONTROLLER_BUTTON_START},
    {"ljoystick",   SDL_CONTROLLER_BUTTON_LEFTSTICK},
    {"rjoystick",   SDL_CONTROLLER_BUTTON_RIGHTSTICK},
    {"ltrigger",    SDL_CONTROLLER_BUTTON_LEFTSHOULDER},
    {"rtrigger",    SDL_CONTROLLER_BUTTON_RIGHTSHOULDER},
    {"up",          SDL_CONTROLLER_BUTTON_DPAD_UP},
    {"down",        SDL_CONTROLLER_BUTTON_DPAD_DOWN},
    {"left",        SDL_CONTROLLER_BUTTON_DPAD_LEFT},
    {"right",       SDL_CONTROLLER_BUTTON_DPAD_RIGHT},
    {"misc",        SDL_CONTROLLER_BUTTON_MISC1},
    {"paddle1",     SDL_CONTROLLER_BUTTON_PADDLE1},
    {"paddle2",     SDL_CONTROLLER_BUTTON_PADDLE2},
    {"paddle3",     SDL_CONTROLLER_BUTTON_PADDLE3},
    {"paddle4",     SDL_CONTROLLER_BUTTON_PADDLE4},
    {"touchpad",    SDL_CONTROLLER_BUTTON_TOUCHPAD},
};

std::unordered_map<std::string, SDL_GameControllerAxis> Input::s_controllerAxisMap = {
    {"leftx",       SDL_CONTROLLER_AXIS_LEFTX},
    {"lefty",       SDL_CONTROLLER_AXIS_LEFTY},
    {"rightx",      SDL_CONTROLLER_AXIS_RIGHTX},
    {"rightx",      SDL_CONTROLLER_AXIS_RIGHTY},
    {"lefttrigger", SDL_CONTROLLER_AXIS_TRIGGERLEFT},
    {"rightTrigger",SDL_CONTROLLER_AXIS_TRIGGERRIGHT},
};

Input::Input() 
    : m_mouseButtonStates{ false, false, false }, m_mousePosition(0, 0), m_windowSize{ 0u,0u }, m_windowResized(false), m_quit(false), 
    m_keysDown{}, m_keysPressed{}, m_keysReleased{}, m_controllerButtonsDown{}, m_controllerButtonsPressed{}, m_controllerButtonsReleased{},
    m_buttonBindings(), m_variableBindings(), m_axisBindings(), m_axis2Bindings(), m_deadzone()
{
    if (SDL_Init(SDL_INIT_JOYSTICK) < 0) {
        DEBUG_ERROR(std::format("SDL initialization failed: {}", SDL_GetError()));
        ASSERT(false);
        return;
    }

    if (SDL_Init(SDL_INIT_GAMECONTROLLER) < 0) {
        DEBUG_ERROR(std::format("SDL initialization failed: {}", SDL_GetError()));
        ASSERT(false);
        return;
    }

    if (SDL_NumJoysticks() < 1 || !SDL_IsGameController(0)) {
        DEBUG_ERROR("No game controller connected.");
        //ASSERT(false);
        return;
    }

    // Open the first joystick
    m_joystick = SDL_JoystickOpen(0);
    if (!m_joystick) {
        DEBUG_ERROR(std::format("Unable to open joystick: {}", SDL_GetError()));
        ASSERT(false);
        return;
    }

    // Open the game controller
    m_gameController = SDL_GameControllerOpen(0);
    if (!m_gameController) {
        DEBUG_ERROR(std::format("Unable to open game controller: {}", SDL_GetError()));
        ASSERT(false);
        return;
    }

    // Print joystick information
    DEBUG_PRINT(std::format("Joystick Name: {}", SDL_JoystickName(m_joystick)));
    DEBUG_PRINT(std::format("Number of Axes: {}", SDL_JoystickNumAxes(m_joystick)));
}

Input::~Input()
{
    SDL_GameControllerClose(m_gameController);
    SDL_JoystickClose(m_joystick);
}

void Input::Update()
{
    // reset the keys pressed and keys released arrays
    m_keysPressed.clear();
    m_keysReleased.clear();
    m_controllerButtonsPressed.clear();
    m_controllerButtonsReleased.clear();

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
#ifdef _DEBUG
        // ImGui events
        ImGui_ImplSDL2_ProcessEvent(&event); // Forward your event to backend
#endif // _DEBUG

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
        case SDL_JOYAXISMOTION:
            // Joystick axis motion event
            //std::cout << "Joystick Axis " << static_cast<int>(event.jaxis.axis) << " Value: " << static_cast<int>(event.jaxis.value) << std::endl;
            break;
        case SDL_CONTROLLERBUTTONDOWN:
            onControllerButtonDown(event);            
            break;
        case SDL_CONTROLLERBUTTONUP:
            onControllerButtonUp(event);            
            break;
        }

    }
}

void Input::LoadInputBindings(const std::string& fileDir, const std::string& levelFile)
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file((fileDir + levelFile).c_str());
    ASSERT(result);

    auto controllerAxis = doc.child("controllerAxis");

    m_deadzone = controllerAxis.attribute("deadzone").as_float();

    for (auto& input : doc.child("inputs").children())
    {
        std::string name(input.attribute("name").as_string());
        std::string inputType(input.attribute("type").as_string());
        if (inputType == "button")
        {
            for (auto& binding : input.child("bindings").children())
            {
                std::string bindingType(binding.attribute("type").as_string());
                std::string value(binding.attribute("value").as_string());
                if (bindingType == "keycode")
                {
                    CreateButtonBinding(name, GetKeycodeFromString(value));
                }
                else if (bindingType == "controllerButton")
                {
                    CreateButtonBinding(name, GetControllerButtonFromString(value));
                }
                else
                {
                    DEBUG_ERROR(std::format("{} is not a valid binding type.", bindingType));
                    ASSERT(false);
                }
            }
        }
        else if (inputType == "variable")
        {
            for (auto& binding : input.child("bindings").children())
            {
                std::string bindingType(binding.attribute("type").as_string());
                std::string value(binding.attribute("value").as_string());
                if (bindingType == "keycode")
                {
                    CreateVairableBinding(name, GetKeycodeFromString(value));
                }
                else if (bindingType == "controllerButton")
                {
                    CreateVairableBinding(name, GetControllerButtonFromString(value));
                }
                else
                {
                    DEBUG_ERROR(std::format("{} is not a valid binding type.", bindingType));
                    ASSERT(false);
                }
            }
        }
        else if (inputType == "axis")
        {
            for (auto& binding : input.child("bindings").children())
            {
                std::string bindingType(binding.attribute("type").as_string());
                std::string value(binding.attribute("value").as_string());
                std::string component(binding.attribute("component").as_string());

                if (bindingType == "keycode")
                {
                    //TODO convert this to a function
                    AxisComponent axis;
                    if (component == "positive")
                        axis = AxisComponent::Positive;
                    else if (component == "negetive")
                        axis = AxisComponent::Negetive;
                    else
                        ASSERT(false);
                    CreateAxisBinding(name, GetKeycodeFromString(value), axis);
                }
                else if (bindingType == "controllerButton")
                {
                    AxisComponent axis;
                    if (component == "positive")
                        axis = AxisComponent::Positive;
                    else if (component == "negetive")
                        axis = AxisComponent::Negetive;
                    else
                        ASSERT(false);
                    CreateAxisBinding(name, GetControllerButtonFromString(value), axis);
                }
                else if (bindingType == "controllerAxis")
                {
                    CreateAxisBinding(name, GetControllerAxisFromString(value));
                }
                else
                {
                    DEBUG_ERROR(std::format("{} is not a valid binding type.", bindingType));
                    ASSERT(false);
                }
            }
        }
        else if (inputType == "axis2")
        {
            for (auto& binding : input.child("bindings").children())
            {
                std::string bindingType(binding.attribute("type").as_string());
                std::string value(binding.attribute("value").as_string());
                std::string component(binding.attribute("component").as_string());

                Axis2Component axis;
                if (component == "up")
                    axis = Axis2Component::Up;
                else if (component == "down")
                    axis = Axis2Component::Down;
                else if (component == "left")
                    axis = Axis2Component::Left;
                else if (component == "right")
                    axis = Axis2Component::Right;
                else
                    ASSERT(false);

                if (bindingType == "keycode")
                {
                    CreateAxis2Binding(name, GetKeycodeFromString(value), axis);
                }
                else if (bindingType == "controllerButton")
                {
                    CreateAxis2Binding(name, GetControllerButtonFromString(value), axis);
                }
                else
                {
                    DEBUG_ERROR(std::format("{} is not a valid binding type.", bindingType));
                    ASSERT(false);
                }
            }
        }
        else
        {
            DEBUG_ERROR(std::format("{} is not a valid input type.", inputType));
            ASSERT(false);
        }
    }
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

bool Input::isControllerButtonDown(SDL_GameControllerButton button)
{
    return m_controllerButtonsDown.count(button);
}

bool Input::isControllerButtonJustPressed(SDL_GameControllerButton button)
{
    return m_controllerButtonsPressed.count(button);
}

bool Input::isControllerButtonJustReleased(SDL_GameControllerButton button)
{
    return m_controllerButtonsReleased.count(button);
}

float Input::getControllerAxisValue(SDL_GameControllerAxis code)
{
    const int minVal = -32768;
    const int maxVal = 32767;

    int value = SDL_JoystickGetAxis(m_joystick, code);
    float normalizedValue = static_cast<float>(value - minVal) / (maxVal - minVal);
    normalizedValue = glm::max(-1.0f, glm::min(1.0f, 2.0f * normalizedValue - 1.0f));

    if (glm::abs(normalizedValue) <= m_deadzone) {
        return 0.0f;  
    }

    return normalizedValue;
}

bool Input::isStringKeyDown(const std::string& input)
{
    SDL_Keycode key = GetKeycodeFromString(input);
    return isKeyDown(key);
}

bool Input::isStringKeyJustPressed(const std::string& input)
{
    SDL_Keycode key = GetKeycodeFromString(input);
    return isKeyJustPressed(key);
}

bool Input::isStringKeyJustReleased(const std::string& input)
{
    SDL_Keycode key = GetKeycodeFromString(input);
    return isKeyJustReleased(key);
}

bool Input::isStringControllerButtonDown(const std::string& input)
{
    SDL_GameControllerButton key = GetControllerButtonFromString(input);
    return isControllerButtonDown(key);
}

bool Input::isStringControllerButtonJustPressed(const std::string& input)
{
    SDL_GameControllerButton key = GetControllerButtonFromString(input);
    return isControllerButtonJustPressed(key);
}

bool Input::isStringControllerButtonJustReleased(const std::string& input)
{
    SDL_GameControllerButton key = GetControllerButtonFromString(input);
    return isControllerButtonJustReleased(key);
}

float Input::getStringControllerAxisValue(const std::string& input)
{
    SDL_GameControllerAxis key = GetControllerAxisFromString(input);
    return getControllerAxisValue(key);
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

void Input::onControllerButtonDown(SDL_Event& event)
{
    SDL_GameControllerButton key = static_cast<SDL_GameControllerButton>(event.cbutton.button);
    m_controllerButtonsDown.insert(key);
    m_controllerButtonsPressed.insert(key);
}

void Input::onControllerButtonUp(SDL_Event& event)
{
    SDL_GameControllerButton key = static_cast<SDL_GameControllerButton>(event.cbutton.button);
    m_controllerButtonsDown.erase(key);
    m_controllerButtonsReleased.insert(key);
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

void Input::CreateButtonBinding(const std::string& input, SDL_Keycode code)
{
    auto it = m_buttonBindings.find(input);
    if (it == m_buttonBindings.end())
        m_buttonBindings[input].keycodes = std::set{ code };
    else
        m_buttonBindings[input].keycodes.insert(code);
}

void Input::CreateButtonBinding(const std::string& input, SDL_GameControllerButton code)
{
    auto it = m_buttonBindings.find(input);
    if (it == m_buttonBindings.end())
        m_buttonBindings[input].controllerButtons = std::set{ code };
    else
        m_buttonBindings[input].controllerButtons.insert(code);
}

void Input::CreateVairableBinding(const std::string& input, SDL_Keycode code)
{
    auto it = m_variableBindings.find(input);
    if (it == m_variableBindings.end())
        m_variableBindings[input].keycodes = std::set{ code };
    else
        m_variableBindings[input].keycodes.insert(code);
}

void Input::CreateVairableBinding(const std::string& input, SDL_GameControllerButton code)
{
    auto it = m_variableBindings.find(input);
    if (it == m_variableBindings.end())
        m_variableBindings[input].controllerButtons = std::set{ code };
    else
        m_variableBindings[input].controllerButtons.insert(code);
}

void Input::CreateAxisBinding(const std::string& input, SDL_Keycode code, AxisComponent axis)
{
    auto it = m_axisBindings.find(input);
    bool hasBinding = it == m_axisBindings.end();
    switch (axis)
    {
    case AxisComponent::Positive:
        if (hasBinding)
            m_axisBindings[input].positive.keycodes = std::set{ code };
        else
            m_axisBindings[input].positive.keycodes.insert(code);
        break;
    case AxisComponent::Negetive:
        if (hasBinding)
            m_axisBindings[input].negetive.keycodes = std::set{ code };
        else
            m_axisBindings[input].negetive.keycodes.insert(code);
        break;
    default:
        ASSERT(false);
    }
}

void Input::CreateAxisBinding(const std::string& input, SDL_GameControllerButton code, AxisComponent axis)
{
    auto it = m_axisBindings.find(input);
    bool hasBinding = it == m_axisBindings.end();
    switch (axis)
    {
    case AxisComponent::Positive:
        if (hasBinding)
            m_axisBindings[input].positive.controllerButtons = std::set{ code };
        else
            m_axisBindings[input].positive.controllerButtons.insert(code);
        break;
    case AxisComponent::Negetive:
        if (hasBinding)
            m_axisBindings[input].negetive.controllerButtons = std::set{ code };
        else
            m_axisBindings[input].negetive.controllerButtons.insert(code);
        break;
    default:
        ASSERT(false);
    }
}

void Input::CreateAxisBinding(const std::string& input, SDL_GameControllerAxis code)
{
    auto it = m_axisBindings.find(input);
    if (it == m_axisBindings.end())
        m_axisBindings[input].controllerAxis = std::set{ code };
    else
        m_axisBindings[input].controllerAxis.insert(code);
}

void Input::CreateAxis2Binding(const std::string& input, SDL_Keycode code, Axis2Component axis)
{
    auto it = m_axis2Bindings.find(input);
    bool hasBinding = it == m_axis2Bindings.end();
    switch (axis)
    {
    case Axis2Component::Up:
        if (hasBinding)
            m_axis2Bindings[input].yAxis.positive.keycodes = std::set{ code };
        else
            m_axis2Bindings[input].yAxis.positive.keycodes.insert(code);
        break;
    case Axis2Component::Down:
        if (hasBinding)
            m_axis2Bindings[input].yAxis.negetive.keycodes = std::set{ code };
        else
            m_axis2Bindings[input].yAxis.negetive.keycodes.insert(code);
        break;
    case Axis2Component::Left:
        if (hasBinding)
            m_axis2Bindings[input].xAxis.negetive.keycodes = std::set{ code };
        else
            m_axis2Bindings[input].xAxis.negetive.keycodes.insert(code);
        break;
    case Axis2Component::Right:
        if (hasBinding)
            m_axis2Bindings[input].xAxis.positive.keycodes = std::set{ code };
        else
            m_axis2Bindings[input].xAxis.positive.keycodes.insert(code);
        break;
    default:
        ASSERT(false);
    }
}

void Input::CreateAxis2Binding(const std::string& input, SDL_GameControllerButton code, Axis2Component axis)
{
    auto it = m_axis2Bindings.find(input);
    bool hasBinding = it == m_axis2Bindings.end();
    switch (axis)
    {
    case Axis2Component::Up:
        if (hasBinding)
            m_axis2Bindings[input].yAxis.positive.controllerButtons = std::set{ code };
        else
            m_axis2Bindings[input].yAxis.positive.controllerButtons.insert(code);
        break;
    case Axis2Component::Down:
        if (hasBinding)
            m_axis2Bindings[input].yAxis.negetive.controllerButtons = std::set{ code };
        else
            m_axis2Bindings[input].yAxis.negetive.controllerButtons.insert(code);
        break;
    case Axis2Component::Left:
        if (hasBinding)
            m_axis2Bindings[input].xAxis.negetive.controllerButtons = std::set{ code };
        else
            m_axis2Bindings[input].xAxis.negetive.controllerButtons.insert(code);
        break;
    case Axis2Component::Right:
        if (hasBinding)
            m_axis2Bindings[input].xAxis.positive.controllerButtons = std::set{ code };
        else
            m_axis2Bindings[input].xAxis.positive.controllerButtons.insert(code);
        break;
    default:
        ASSERT(false);
    }
}

void Input::CreateAxis2Binding(const std::string& input, SDL_GameControllerAxis code, Axis2Direction axis)
{
    auto it = m_axis2Bindings.find(input);
    bool hasBinding = it == m_axis2Bindings.end();
    switch (axis)
    {
    case Axis2Direction::X:
        if (hasBinding)
            m_axis2Bindings[input].xAxis.controllerAxis = std::set{ code };
        else
            m_axis2Bindings[input].xAxis.controllerAxis.insert(code);
        break;
    case Axis2Direction::Y:
        if (hasBinding)
            m_axis2Bindings[input].yAxis.controllerAxis = std::set{ code };
        else
            m_axis2Bindings[input].yAxis.controllerAxis.insert(code);
        break;
    default:
        ASSERT(false);
    }
}

bool Input::isInputDown(const std::string& input)
{
    auto keys = m_buttonBindings.find(input);
    ASSERT(keys != m_buttonBindings.end());
    for (auto it = keys->second.keycodes.begin(); it != keys->second.keycodes.end(); ++it)
    {
        if (isKeyDown(*it))
            return true;
    }
    for (auto it = keys->second.controllerButtons.begin(); it != keys->second.controllerButtons.end(); ++it)
    {
        if (isControllerButtonDown(*it))
            return true;
    }
    return false;
}

bool Input::isInputJustPressed(const std::string& input)
{
    auto keys = m_buttonBindings.find(input);
    ASSERT(keys != m_buttonBindings.end());
    for (auto it = keys->second.keycodes.begin(); it != keys->second.keycodes.end(); ++it)
    {
        if (isKeyJustPressed(*it))
            return true;
    }
    for (auto it = keys->second.controllerButtons.begin(); it != keys->second.controllerButtons.end(); ++it)
    {
        if (isControllerButtonJustPressed(*it))
            return true;
    }
    return false;
}

bool Input::isInputJustReleased(const std::string& input)
{
    auto keys = m_buttonBindings.find(input);
    ASSERT(keys != m_buttonBindings.end());
    for (auto it = keys->second.keycodes.begin(); it != keys->second.keycodes.end(); ++it)
    {
        if (isKeyJustReleased(*it))
            return true;
    }
    for (auto it = keys->second.controllerButtons.begin(); it != keys->second.controllerButtons.end(); ++it)
    {
        if (isControllerButtonJustReleased(*it))
            return true;
    }
    return false;
}

float Input::getInputVariable(const std::string& input)
{
    auto keys = m_variableBindings.find(input);
    ASSERT(keys != m_variableBindings.end());
    for (auto it = keys->second.keycodes.begin(); it != keys->second.keycodes.end(); ++it)
    {
        if (isKeyDown(*it))
            return 1.f;
    }
    for (auto it = keys->second.controllerButtons.begin(); it != keys->second.controllerButtons.end(); ++it)
    {
        if (isControllerButtonDown(*it))
            return 1.f;
    }
    return 0.f;
}

float Input::getInputAxis(const std::string& input)
{
    auto keys = m_axisBindings.find(input);
    ASSERT(keys != m_axisBindings.end());

    float positive = 0.f;
    for (auto it = keys->second.positive.keycodes.begin(); it != keys->second.positive.keycodes.end(); ++it)
    {
        if (isKeyDown(*it))
            positive = 1.f;
    }
    for (auto it = keys->second.positive.controllerButtons.begin(); it != keys->second.positive.controllerButtons.end(); ++it)
    {
        if (isControllerButtonDown(*it))
            positive = 1.f;
    }

    float negetive = 0.f;
    for (auto it = keys->second.negetive.keycodes.begin(); it != keys->second.negetive.keycodes.end(); ++it)
    {
        if (isKeyDown(*it))
            negetive = 1.f;
    }
    for (auto it = keys->second.negetive.controllerButtons.begin(); it != keys->second.negetive.controllerButtons.end(); ++it)
    {
        if (isControllerButtonDown(*it))
            negetive = 1.f;
    }

    float value = positive - negetive;

    if (!value)
    {
        for (auto it = keys->second.controllerAxis.begin(); it != keys->second.controllerAxis.end(); ++it)
        {
            value += getControllerAxisValue(*it);
        }
        value /= (float)keys->second.controllerAxis.size();
    }
    return value;
}

glm::vec2 Input::getInputAxis2(const std::string& input)
{
    auto keys = m_axis2Bindings.find(input);
    ASSERT(keys != m_axis2Bindings.end());

    float up = 0.f;
    for (auto it = keys->second.yAxis.positive.keycodes.begin(); it != keys->second.yAxis.positive.keycodes.end(); ++it)
    {
        if (isKeyDown(*it))
            up = 1.f;
    }
    for (auto it = keys->second.yAxis.positive.controllerButtons.begin(); it != keys->second.yAxis.positive.controllerButtons.end(); ++it)
    {
        if (isControllerButtonDown(*it))
            up = 1.f;
    }

    float down = 0.f;
    for (auto it = keys->second.yAxis.negetive.keycodes.begin(); it != keys->second.yAxis.negetive.keycodes.end(); ++it)
    {
        if (isKeyDown(*it))
            down = 1.f;
    }
    for (auto it = keys->second.yAxis.negetive.controllerButtons.begin(); it != keys->second.yAxis.negetive.controllerButtons.end(); ++it)
    {
        if (isControllerButtonDown(*it))
            down = 1.f;
    }
    
    float left = 0.f;
    for (auto it = keys->second.xAxis.negetive.keycodes.begin(); it != keys->second.xAxis.negetive.keycodes.end(); ++it)
    {
        if (isKeyDown(*it))
            left = 1.f;
    }
    for (auto it = keys->second.xAxis.negetive.controllerButtons.begin(); it != keys->second.xAxis.negetive.controllerButtons.end(); ++it)
    {
        if (isControllerButtonDown(*it))
            left = 1.f;
    }

    float right = 0.f;
    for (auto it = keys->second.xAxis.positive.keycodes.begin(); it != keys->second.xAxis.positive.keycodes.end(); ++it)
    {
        if (isKeyDown(*it))
            right = 1.f;
    }
    for (auto it = keys->second.xAxis.positive.controllerButtons.begin(); it != keys->second.xAxis.positive.controllerButtons.end(); ++it)
    {
        if (isControllerButtonDown(*it))
            right = 1.f;
    }

    return glm::vec2{ right - left, up - down };
}

SDL_Keycode Input::GetKeycodeFromString(const std::string& input)
{
    auto it = s_keycodeMap.find(input);
    ASSERT(it != s_keycodeMap.end());
    return it->second;
}

SDL_GameControllerButton Input::GetControllerButtonFromString(const std::string& input)
{
    auto it = s_controllerButtonMap.find(input);
    ASSERT(it != s_controllerButtonMap.end());
    return it->second;
}

SDL_GameControllerAxis Input::GetControllerAxisFromString(const std::string& input)
{
    auto it = s_controllerAxisMap.find(input);
    ASSERT(it != s_controllerAxisMap.end());
    return it->second;
}

void Input::onMouseButtonDown(SDL_Event& event)
{
    if (event.button.button == SDL_BUTTON_LEFT)
    {
        m_mouseButtonStates[(int)MouseButtons::Left] = true;
    }
    if (event.button.button == SDL_BUTTON_MIDDLE)
    {
        m_mouseButtonStates[(int)MouseButtons::Middle] = true;
    }
    if (event.button.button == SDL_BUTTON_RIGHT)
    {
        m_mouseButtonStates[(int)MouseButtons::Right] = true;
    }
}

void Input::onMouseMove(SDL_Event& event)
{
    m_mousePosition.x = (float)event.motion.x;
    m_mousePosition.y = (float)event.motion.y;
}

void Input::onMouseButtonUp(SDL_Event& event)
{
    if (event.button.button == SDL_BUTTON_LEFT)
    {
        m_mouseButtonStates[(int)MouseButtons::Left] = false;
    }
    if (event.button.button == SDL_BUTTON_MIDDLE)
    {
        m_mouseButtonStates[(int)MouseButtons::Middle] = false;
    }
    if (event.button.button == SDL_BUTTON_RIGHT)
    {
        m_mouseButtonStates[(int)MouseButtons::Right] = false;
    }
}

bool Input::getMouseButtonState(int buttonNumber)
{
    return m_mouseButtonStates[buttonNumber];
}

void Input::LUABind(sol::state& lua)
{
    lua.new_usertype<Input>("Input",
        "isInputDown", &Input::isInputDown,
        "isInputJustPressed", &Input::isInputJustPressed,
        "isInputJustReleased", &Input::isInputJustReleased,
        "isKeyDown", &Input::isStringKeyDown,
        "isKeyJustPressed", &Input::isStringKeyJustPressed,
        "isKeyJustReleased", &Input::isStringKeyJustReleased,
        "isStringControllerButtonDown", &Input::isStringControllerButtonDown,
        "isStringControllerButtonJustPressed", &Input::isStringControllerButtonJustPressed,
        "isStringControllerButtonJustReleased", &Input::isStringControllerButtonJustReleased,
        "getInputVariable", &Input::getInputVariable,
        "getInputAxis", &Input::getInputAxis,
        "getInputAxis2", &Input::getInputAxis2
    );
    lua.set("Input", this);
}