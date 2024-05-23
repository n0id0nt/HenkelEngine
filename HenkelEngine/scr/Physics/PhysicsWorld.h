#pragma once

#include "glm\glm.hpp"
#include "box2d\box2d.h"
#include "ContactListener.h"

class PhysicsWorld
{
public:
	PhysicsWorld(glm::vec2 gravity, float timeStep, int velocityIterations, int positionIterations, float pixelsPerMeter);

	void Step();

	void ClearForces();

	b2Body* CreateBody(const b2BodyDef* bodyDef);
	void DestroyBody(b2Body* body);

	float GetPixelsPerMeter() const;
	void SetPixelsPerMeter(float pixelsPerMeter);

private:

	ContactListener m_contactListener;

	float m_pixelsPerMeter;

	b2World m_world;

	float m_timeStep;
	int m_velocityIteration;
	int m_positionIterations;
};

