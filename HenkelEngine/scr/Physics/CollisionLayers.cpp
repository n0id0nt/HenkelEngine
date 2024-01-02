#include "CollisionLayers.h"

CollisionLayers::CollisionLayers() : m_layers()
{
	m_layers["None"]	= 0u;
	m_layers["Ground"]	= 1u << 0;
	m_layers["Player"]	= 1u << 1;
	m_layers["Ladder"]	= 1u << 2;
	m_layers["Water"]	= 1u << 3;
	m_layers["All"]		= m_layers["Ground"] | m_layers["Player"] | m_layers["Ladder"] | m_layers["Water"];
}

unsigned int CollisionLayers::GetLayer(std::string layerName)
{
	return m_layers[layerName];
}
