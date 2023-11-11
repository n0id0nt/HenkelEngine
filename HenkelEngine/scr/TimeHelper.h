#pragma once

#include "LUABindable.h"

class Time : public LUABindable
{
public:
	Time();

	void SetTime(const float& time);
	float GetTime();

	void LUABind(sol::state& lua) override;

private:
	float m_time;
};

