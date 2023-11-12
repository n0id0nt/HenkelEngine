#pragma once

#include "LUABindable.h"

class Time : public LUABindable
{
public:
	Time();

	void SetTime(const float& time);
	float GetTime();
	
	void SetDeltaTime(const float& deltaTime);
	float GetDeltaTime();

	void LUABind(sol::state& lua) override;

private:
	float m_time;
	float m_deltaTime;
};

