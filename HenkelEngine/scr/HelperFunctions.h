#pragma once
#include <string>
#include <regex>
#include "sol\sol.hpp"
#include "glm\glm.hpp"
#include "Physics/ContactListener.h"
#include "opengl/openglHelper.h"

namespace HenkelEngine
{
    static std::string ltrim(const std::string& s) {
        return std::regex_replace(s, std::regex("^\\s+"), std::string(""));
    }

    static std::string rtrim(const std::string& s) {
        return std::regex_replace(s, std::regex("\\s+$"), std::string(""));
    }

    static std::string trim(const std::string& s) {
        return ltrim(rtrim(s));
    }

    static void LUABindLibraries(sol::state& lua)
    {
        lua.new_usertype<glm::vec2>("vec2",
            "new", sol::constructors<glm::vec2(float, float)>(),
            "x", &glm::vec2::x,
            "y", &glm::vec2::y);
        lua["vec2"]["copy"] = sol::overload(
            [](const glm::vec2& original) {
                return glm::vec2(original);
            }
        );
        lua.new_usertype<glm::vec3>("vec3",
            "new", sol::constructors<glm::vec3(float, float, float)>(),
            "x", &glm::vec3::x,
            "y", &glm::vec3::y,
            "z", &glm::vec3::z);
        lua["vec3"]["copy"] = sol::overload(
            [](const glm::vec3& original) {
                return glm::vec3(original);
            }
        );
        lua.new_usertype<ContactListener::Contact>("contact",
            "other", &ContactListener::Contact::other,
            "contactPoints", &ContactListener::Contact::contactPoints,
            "impulses", &ContactListener::Contact::impulses,
            "normal", &ContactListener::Contact::normal,
            "velocity", &ContactListener::Contact::velocity);
        lua.set("print", [](const std::string& original) { DEBUG_PRINT(original); });
    }

    static std::string GetFileDir(const std::string& filePath)
    {
        std::string fileDir = filePath;
        auto lastSlash = fileDir.find_last_of('/');
        return fileDir.replace(fileDir.begin() + lastSlash + 1, fileDir.end(), "");
    }

    static double grad(int hash, double value) {
        int h = hash & 15;
        double grad = 1 + (h & 7); // Gradient value 1-8
        if (h & 8) 
            grad = -grad;   // Randomly invert half of them
        return (grad * value);
    }

    static double PerlinNoise(unsigned int seed, double value)
    {
        int p[512];
        srand(seed);
        for (int i = 0; i < 256; ++i) {
            p[i] = p[256 + i] = rand() % 256;
        }

        int X = static_cast<int>(glm::floor(value)) & 255;
        value -= glm::floor(value);

        double u = glm::smoothstep(0.0, 1.0, value);

        int a = p[X];
        int b = p[X + 1];

        return glm::mix(u, grad(a, value), grad(b, value - 1));
    }

    static glm::vec4 HexColorToVec4(std::string colorString)
    {
        std::regex pattern("#?([0-9a-fA-F]{2})([0-9a-fA-F]{2})([0-9a-fA-F]{2})([0-9a-fA-F]{2})");

        std::smatch match;
        ASSERT(std::regex_match(colorString, match, pattern));

        auto r = std::stoul(match[1].str(), nullptr, 16);
        auto g = std::stoul(match[2].str(), nullptr, 16);
        auto b = std::stoul(match[3].str(), nullptr, 16);
        auto a = std::stoul(match[4].str(), nullptr, 16);

        return glm::vec4(r / 255.0f, b / 255.0f, g / 255.0f, a / 255.0f);
    }
}
