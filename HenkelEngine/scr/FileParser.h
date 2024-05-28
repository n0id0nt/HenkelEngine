#pragma once

#include "World.h"
#include "ECS\Entity\Entity.h"
#include "pugixml.hpp"
#include "Resourse\TileSheet.h"
#include <memory>
#include "UI\UIArea.h"

namespace FileParser
{
	void LoadWorld(World* world, const std::string& fileDir, const std::string& worldFile);
	void LoadLevel(World* world, Entity* levelEntity);

	Entity* LoadTemplate(World* world, const std::string& fileDir, const std::string& levelFile);
	Entity* CreateObject(World* world, const pugi::xml_node& object, const std::string& fileDir, const TileSheet& tileSheet);
	Entity* CreateTemplatedObject(World* world, const std::string& levelFile);

	Entity* LoadUILayout(World* world, const std::string& fileDir, const std::string& levelFile);
	std::unique_ptr<UIArea> CreateUIElement(World* world, const pugi::xml_node& object, const std::string& fileDir);
};

