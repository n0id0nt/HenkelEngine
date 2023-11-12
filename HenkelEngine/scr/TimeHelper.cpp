#include "TimeHelper.h"

Time::Time() : m_time(0.f), m_deltaTime(0.f)
{
}

void Time::SetTime(const float& time)
{
    m_time = time;
}

float Time::GetTime()
{
	return m_time;
}

void Time::SetDeltaTime(const float& deltaTime)
{
    m_deltaTime = deltaTime;
}

float Time::GetDeltaTime()
{
    return m_deltaTime;
}

void Time::LUABind(sol::state& lua)
{
    lua.new_usertype<Time>("Time",
        "getTime", &Time::GetTime,
        "getDeltaTime", &Time::GetDeltaTime
    );
    lua.set("Time", this);
}
