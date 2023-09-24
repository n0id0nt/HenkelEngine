#pragma once

class Panel;

class IRenderer
{
public:
	virtual void Render(Panel* panel) = 0;
};