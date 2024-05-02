#pragma once
#include "LUABindable.h"
#include "sol\sol.hpp"
#include <ECS\Entity\Entity.h>

class Messenger : public LUABindable
{
public:

	void Send(const std::string& messageId, const sol::object& message, const Entity& sender);

	void LUABind(sol::state& lua) override;

private:

	struct Message
	{
		std::string messageId;
		sol::object message;
		Entity* sender;
	};

	std::vector<Message> m_messages;
};

