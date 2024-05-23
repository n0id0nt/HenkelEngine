#pragma once

#include <GL/glew.h>
#include <string>
#include <format>

#define DEBUG_PRINT(x) DebugPrint(std::format("PRINT: {}\n",x));
#define DEBUG_ERROR(x) DebugPrint(std::format("ERROR: {}\n",x));

#ifdef _DEBUG
#define ASSERT(x) if (!(x)) __debugbreak();
#else
#define ASSERT(x)
#endif // _DEBUG

#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__));

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);
void DebugPrint(const std::string& message);