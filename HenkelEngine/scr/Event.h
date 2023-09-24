#pragma once

#include <vector>

template <class Member, typename... Types>
class Event
{
public:
	typedef void (Member::*Function)(Types...);

	Event() : m_functions()
	{}

	void AddListener(Member* member, Function func)
	{
		m_functions.push_back(std::make_pair(member, func));
	}

	void RemoveListener(Member* member, Function func)
	{
		remove(m_functions.begin(), m_functions.end(), std::make_pair(member, func));
	}

	void Invoke(Types... types)
	{
		for (std::pair<Member*, Function> func : m_functions)
		{
			(func.first->*func.second)(types...);
		}
	}

private:
	std::vector<std::pair<Member*, Function>> m_functions;
};

