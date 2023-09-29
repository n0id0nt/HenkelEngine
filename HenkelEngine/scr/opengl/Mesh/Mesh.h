#pragma once

#include <vector>
#include "glm\glm.hpp"

struct Vertex;

class Mesh
{
public:
	static void setCubeData(std::vector<Vertex>& vertices);
	static void setQuadData(std::vector<Vertex>& vertices);
};

