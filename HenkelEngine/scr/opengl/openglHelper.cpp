#include "openglHelper.h"

#include <iostream>
#include <Windows.h>
#include <codecvt>
#include <format>

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR) 
    { 
        std::string error(reinterpret_cast<const char*>(glGetError()));
        DEBUG_PRINT(error);
    }
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::string errorMessage = std::format("[OpenGL Error] ({}): {} {}:{}", error, function, file, line);
        DEBUG_PRINT(errorMessage)
        return false;
    }
    return true;
}

void DebugPrint(const std::string& message)
{
    std::wstring stemp = std::wstring(message.begin(), message.end());
    LPCWSTR sw = stemp.c_str();
    OutputDebugString(sw);
}
