#include "TimeHelper.h"

Time::Time() : m_time(0.f)
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

void Time::LUABind(sol::state& lua)
{
    lua.new_usertype<Time>("Time",
        "getTime", &Time::GetTime
    );
    lua.set("Time", this);
}
