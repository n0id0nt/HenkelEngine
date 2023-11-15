#pragma once

#include <unordered_map>
#include <string>
#include "sol\sol.hpp"

struct SpriteAnimation
{
	unsigned int startFrame;
	unsigned int endFrame;
	float animationTime;
	bool loop;
};

class SpriteAnimationComponent
{
public:
	SpriteAnimationComponent(std::unordered_map<std::string, SpriteAnimation> animations, std::string startAnimation);

	int GetCurrentFrame();

	void PlayAnimation(std::string animation);
	std::string GetCurrentAnimation();
	bool IsAnimationPlaying();

	static void LUABind(sol::state& lua);

private:
	std::unordered_map<std::string, SpriteAnimation> m_animations;

	std::string m_curAnimation;
	float m_animationStartTime;

};

