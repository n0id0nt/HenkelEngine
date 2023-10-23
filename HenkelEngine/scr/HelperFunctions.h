#pragma once
#include <string>
#include <regex>
#include "sol\sol.hpp"
#include "glm\glm.hpp"

namespace HenkelEngine
{
    std::string ltrim(const std::string& s) {
        return std::regex_replace(s, std::regex("^\\s+"), std::string(""));
    }

    std::string rtrim(const std::string& s) {
        return std::regex_replace(s, std::regex("\\s+$"), std::string(""));
    }

    std::string trim(const std::string& s) {
        return ltrim(rtrim(s));
    }

    void LUABindLibraries(sol::state& lua)
    {
        lua.new_usertype<glm::vec2>("vec2",
            "x", &glm::vec2::x,
            "y", &glm::vec2::y);
        lua.new_usertype<glm::vec3>("vec3",
            "x", &glm::vec3::x,
            "y", &glm::vec3::y,
            "z", &glm::vec3::z);
    }
}
