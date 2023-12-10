#pragma once
#include <string>
#include <regex>
#include "sol\sol.hpp"
#include "glm\glm.hpp"

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
    }

    static std::string GetFileDir(const std::string& filePath)
    {
        std::string fileDir = filePath;
        auto lastSlash = fileDir.find_last_of('/');
        return fileDir.replace(fileDir.begin() + lastSlash + 1, fileDir.end(), "");
    }
}
