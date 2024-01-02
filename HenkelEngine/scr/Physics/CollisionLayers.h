#pragma once

#include <unordered_map>
#include <string>

class CollisionLayers
{
public:
	CollisionLayers();

	unsigned int GetLayer(std::string layerName);

private:
	std::unordered_map<std::string, unsigned int> m_layers;

};

