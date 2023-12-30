#pragma once

#include "../HenkelEngine.h"

class PhysicsWorld
{
public:
	PhysicsWorld(glm::vec2 gravity, float timeStep, int velocityIterations, int positionIterations, float pixelsPerMeter);

	void Step();

	void ClearForces();

	b2Body* CreateBody(const b2BodyDef* bodyDef);
	void DestroyBody(b2Body* body);

	void SetContactListener(b2ContactListener* contactListener);

	float GetPixelsPerMeter() const;

private:

	float m_pixelsPerMeter;

	b2World m_world;

	float m_timeStep;
	int m_velocityIteration;
	int m_positionIterations;
};

