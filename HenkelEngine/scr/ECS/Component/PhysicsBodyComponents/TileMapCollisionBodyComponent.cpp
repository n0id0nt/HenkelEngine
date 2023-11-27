#include "TileMapCollisionBodyComponent.h"

bool TileMapCollisionBodyComponent::IsTileClearAtDir(glm::ivec2 tile, const TileMapComponent& tilemap, Dir inputDir)
{
	switch (inputDir)
	{
	case Dir::Left:
		return tile.x - 1 < 0 || tilemap.GetTile(tile.y, tile.x - 1) == 0;
	case Dir::Up:
		return tile.y - 1 < 0 || tilemap.GetTile(tile.y - 1, tile.x) == 0;
	case Dir::Right:
		return tile.x >= tilemap.GetWidth() - 1 || tilemap.GetTile(tile.y, tile.x + 1) == 0;
	case Dir::Down:
		return tile.y >= tilemap.GetHeight() - 1 || tilemap.GetTile(tile.y + 1, tile.x) == 0;
	}
	return false;
}

TileMapCollisionBodyComponent::Dir TileMapCollisionBodyComponent::NextDir(Dir inputDir)
{
	return static_cast<Dir>(((int)inputDir + 1) % (int)Dir::Count);
}

TileMapCollisionBodyComponent::Dir TileMapCollisionBodyComponent::PreviousDir(Dir inputDir)
{
	int newDir = (int)inputDir - 1;
	if (newDir < 0)
	{
		newDir = (int)Dir::Count - 1;
	}
	return static_cast<Dir>(newDir);
}

bool TileMapCollisionBodyComponent::IsNextDirClear(glm::ivec2 tile, const TileMapComponent& tilemap, Dir inputDir)
{
	return IsTileClearAtDir(tile, tilemap, NextDir(inputDir));
}

glm::ivec2 TileMapCollisionBodyComponent::GetTileInDir(glm::ivec2 tile, Dir inputDir)
{
	switch (inputDir)
	{
	case Dir::Left:
		tile.x -= 1;
		break;
	case Dir::Up:
		tile.y -= 1;
		break;
	case Dir::Right:
		tile.x += 1;
		break;
	case Dir::Down:
		tile.y += 1;
		break;
	}
	return tile;
}

std::array<b2Vec2, 2> TileMapCollisionBodyComponent::GetSideLine(Dir inputDir, glm::ivec2 inputTile, const TileMapComponent& tilemap)
{
	switch (inputDir)
	{
	case Dir::Right:
		return std::array<b2Vec2, 2>{
			b2Vec2{ (inputTile.x * tilemap.GetTileWidth() + tilemap.GetTileWidth() / 2.f) / m_world->GetPixelsPerMeter(), (inputTile.y * tilemap.GetTileWidth() - tilemap.GetTileWidth() / 2.f) / m_world->GetPixelsPerMeter() },
			b2Vec2{ (inputTile.x * tilemap.GetTileWidth() + tilemap.GetTileWidth() / 2.f) / m_world->GetPixelsPerMeter(), (inputTile.y * tilemap.GetTileWidth() + tilemap.GetTileWidth() / 2.f) / m_world->GetPixelsPerMeter() },
		};
	case Dir::Up:
		return std::array<b2Vec2, 2>{
			b2Vec2{ (inputTile.x * tilemap.GetTileWidth() - tilemap.GetTileWidth() / 2.f) / m_world->GetPixelsPerMeter(), (inputTile.y * tilemap.GetTileWidth() - tilemap.GetTileWidth() / 2.f) / m_world->GetPixelsPerMeter() },
			b2Vec2{ (inputTile.x * tilemap.GetTileWidth() + tilemap.GetTileWidth() / 2.f) / m_world->GetPixelsPerMeter(), (inputTile.y * tilemap.GetTileWidth() - tilemap.GetTileWidth() / 2.f) / m_world->GetPixelsPerMeter() },
		};
	case Dir::Left:
		return std::array<b2Vec2, 2>{
			b2Vec2{ (inputTile.x * tilemap.GetTileWidth() - tilemap.GetTileWidth() / 2.f) / m_world->GetPixelsPerMeter(), (inputTile.y * tilemap.GetTileWidth() + tilemap.GetTileWidth() / 2.f) / m_world->GetPixelsPerMeter() },
			b2Vec2{ (inputTile.x * tilemap.GetTileWidth() - tilemap.GetTileWidth() / 2.f) / m_world->GetPixelsPerMeter(), (inputTile.y * tilemap.GetTileWidth() - tilemap.GetTileWidth() / 2.f) / m_world->GetPixelsPerMeter() },
		};
	case Dir::Down:
		return std::array<b2Vec2, 2>{
			b2Vec2{ (inputTile.x * tilemap.GetTileWidth() + tilemap.GetTileWidth() / 2.f) / m_world->GetPixelsPerMeter(), (inputTile.y * tilemap.GetTileWidth() + tilemap.GetTileWidth() / 2.f) / m_world->GetPixelsPerMeter() },
			b2Vec2{ (inputTile.x * tilemap.GetTileWidth() - tilemap.GetTileWidth() / 2.f) / m_world->GetPixelsPerMeter(), (inputTile.y * tilemap.GetTileWidth() + tilemap.GetTileWidth() / 2.f) / m_world->GetPixelsPerMeter() },
		};
	}
	return std::array<b2Vec2, 2>{};
}

void TileMapCollisionBodyComponent::CreateLoop(Dir inputDir, glm::ivec2 inputTile, std::unordered_set<glm::ivec2, IVec2Hash>& checkedTiles, const TileMapComponent& tilemap)
{
	std::vector<b2Vec2> vertices;

	b2FixtureDef fixtureDef;
	fixtureDef.friction = 0.f;
	fixtureDef.density = 1.f;

	b2ChainShape chainShape;

	std::array<b2Vec2, 2> line = GetSideLine(inputDir, inputTile, tilemap);
	vertices.insert(vertices.end(), line.begin(), line.end());
	inputDir = NextDir(inputDir);

	while (!vertices.empty() && vertices.front() != vertices.back())
	{
		if (IsTileClearAtDir(inputTile, tilemap, inputDir))
		{
			line = GetSideLine(inputDir, inputTile, tilemap);
			vertices.push_back(line.back());
			inputDir = NextDir(inputDir);
		}
		else
		{
			inputTile = GetTileInDir(inputTile, inputDir);
			checkedTiles.insert(inputTile);
			inputDir = PreviousDir(inputDir);
		}
	}

	vertices.pop_back();

	chainShape.CreateLoop(vertices.data(), vertices.size());
	fixtureDef.shape = &chainShape;

	m_body->CreateFixture(&fixtureDef);
}

TileMapCollisionBodyComponent::TileMapCollisionBodyComponent(PhysicsWorld* world, const TileMapComponent& tilemap) 
	: m_world(world)
{
	b2BodyDef bodyDef;
	bodyDef.fixedRotation = true;
	bodyDef.type = b2_staticBody;
	bodyDef.position = b2Vec2(bodyDef.position.x / m_world->GetPixelsPerMeter(), bodyDef.position.y / m_world->GetPixelsPerMeter());
	//bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(gameObjectEntity);
	m_body = m_world->CreateBody(&bodyDef);	

	std::unordered_set<glm::ivec2, IVec2Hash> checkedTiles;

	for (unsigned int row = 0; row < tilemap.GetHeight(); row++)
	{
		for (unsigned int col = 0; col < tilemap.GetWidth(); col++)
		{
			unsigned int tile = tilemap.GetTile(row, col);
			glm::ivec2 tilePos{col, row};
			if (tile != 0 && !checkedTiles.contains(tilePos))
			{
				checkedTiles.insert(tilePos);
				if (IsTileClearAtDir(tilePos, tilemap, Dir::Left))
				{
					CreateLoop(Dir::Left, tilePos, checkedTiles, tilemap);
				}
				else if (IsTileClearAtDir(tilePos, tilemap, Dir::Up))
				{
					CreateLoop(Dir::Up, tilePos, checkedTiles, tilemap);
				}
				else if (IsTileClearAtDir(tilePos, tilemap, Dir::Right))
				{
					CreateLoop(Dir::Right, tilePos, checkedTiles, tilemap);
				}
				else if (IsTileClearAtDir(tilePos, tilemap, Dir::Down))
				{
					CreateLoop(Dir::Down, tilePos, checkedTiles, tilemap);
				}
			}
		}
	}	
}

TileMapCollisionBodyComponent::~TileMapCollisionBodyComponent()
{
	m_world->DestroyBody(m_body);
}

glm::vec2 TileMapCollisionBodyComponent::GetPosition()
{
	b2Vec2 pos = m_body->GetPosition();
	return glm::vec2(pos.x * m_world->GetPixelsPerMeter(), pos.y * m_world->GetPixelsPerMeter());
}

void TileMapCollisionBodyComponent::SetPosition(glm::vec2 pos)
{
	m_body->SetTransform(b2Vec2(pos.x / m_world->GetPixelsPerMeter(), pos.y / m_world->GetPixelsPerMeter()), 0.f);
}

b2Body* TileMapCollisionBodyComponent::GetBody() const
{
	return m_body;
}


