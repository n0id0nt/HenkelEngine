#pragma once

#include <unordered_map>
#include <string>

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

	void PlayAnimation(std::string animation);
	int GetCurrentFrame();

private:
	std::unordered_map<std::string, SpriteAnimation> m_animations;

	std::string m_curAnimation;
	float m_animationStartTime;

};

