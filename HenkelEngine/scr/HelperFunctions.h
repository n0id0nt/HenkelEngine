#pragma once
#include <string>
#include <regex>
#include "sol\sol.hpp"
#include "glm\glm.hpp"
#include "Physics/ContactListener.h"
#include "opengl/openglHelper.h"
#include "imgui.h"

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

    static float GetHighestPointOfFixture(b2Fixture* fixture)
    {
        float highestY = -std::numeric_limits<float>::infinity();
        b2Shape* shape = fixture->GetShape();
        switch (shape->GetType())
        {
        case b2Shape::e_polygon:
        {
            b2PolygonShape* polygonShape = dynamic_cast<b2PolygonShape*>(shape);
            ASSERT(polygonShape);
            b2Body* fixtureBody = fixture->GetBody();
            for (int i = 0; i < polygonShape->m_count; ++i)
            {
                b2Vec2 vertex = fixtureBody->GetWorldPoint(polygonShape->m_vertices[i]);
                if (highestY < vertex.y)
                {
                    highestY = vertex.y;
                }
            }
            break;
        }
        case b2Shape::e_chain:
        {
            b2ChainShape* chainShapeB = dynamic_cast<b2ChainShape*>(shape);
            ASSERT(chainShapeB);
            b2Body* fixtureBody = fixture->GetBody();
            for (int i = 0; i < chainShapeB->m_count; ++i)
            {
                b2Vec2 vertex = fixtureBody->GetWorldPoint(chainShapeB->m_vertices[i]);
                if (highestY < vertex.y)
                {
                    highestY = vertex.y;
                }
            }
            break;
        }
        default:
            ASSERT(false);
            break;
        }
        return highestY;
    }

    static float GetLowestPointOfFixture(b2Fixture* fixture)
    {
        float lowestY = std::numeric_limits<float>::infinity();
        b2Shape* shape = fixture->GetShape();
        switch (shape->GetType())
        {
        case b2Shape::e_polygon:
        {
            b2PolygonShape* polygonShape = dynamic_cast<b2PolygonShape*>(shape);
            ASSERT(polygonShape);
            b2Body* fixtureBody = fixture->GetBody();
            for (int i = 0; i < polygonShape->m_count; ++i)
            {
                b2Vec2 vertex = fixtureBody->GetWorldPoint(polygonShape->m_vertices[i]);
                if (lowestY > vertex.y)
                {
                    lowestY = vertex.y;
                }
            }
            break;
        }
        case b2Shape::e_chain:
        {
            b2ChainShape* chainShapeB = dynamic_cast<b2ChainShape*>(shape);
            ASSERT(chainShapeB);
            b2Body* fixtureBody = fixture->GetBody();
            for (int i = 0; i < chainShapeB->m_count; ++i)
            {
                b2Vec2 vertex = fixtureBody->GetWorldPoint(chainShapeB->m_vertices[i]);
                if (lowestY > vertex.y)
                {
                    lowestY = vertex.y;
                }
            }
            break;
        }
        default:
            ASSERT(false);
            break;
        }
        return lowestY;
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

    static void SetDarkThemeColors()
    {
        auto primaryColor = ImVec4{ 0.1f, 0.505f, 0.11f, 1.0f };
        auto primaryColorHighlight = ImVec4{ 0.15f, 0.605f, 0.18f, 1.0f };
        auto secondaryColor = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };
        auto bgColor = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

        auto& colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_WindowBg] = bgColor;

        // Headers
        colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Buttons
        colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Frame BG
        colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Tabs
        colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
        colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
        colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

        // Title
        colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Slider
        colors[ImGuiCol_SliderGrab] = primaryColor;
        colors[ImGuiCol_SliderGrabActive] = primaryColorHighlight;

        // Check Mark
        colors[ImGuiCol_CheckMark] = primaryColorHighlight;

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
            "getHighestPointOfFixture", [](const ContactListener::Contact& contact) { return GetHighestPointOfFixture(contact.fixture); },
            "getHighestPointOfOtherFixture", [](const ContactListener::Contact& contact) { return GetHighestPointOfFixture(contact.otherFixture); },
            "getLowestPointOfFixture", [](const ContactListener::Contact& contact) { return GetLowestPointOfFixture(contact.fixture); },
            "getLowestPointOfOtherFixture", [](const ContactListener::Contact& contact) { return GetLowestPointOfFixture(contact.otherFixture); },
            "other", &ContactListener::Contact::other,
            "contactPoints", &ContactListener::Contact::contactPoints,
            "impulses", &ContactListener::Contact::impulses,
            "normal", &ContactListener::Contact::normal,
            "velocity", &ContactListener::Contact::velocity,
            "enabled", &ContactListener::Contact::enabled);
        lua.set("print", [](const std::string& original) { DEBUG_PRINT(original); });
    }
}
