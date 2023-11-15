#include "SpriteAnimationComponent.h"
#include "Engine.h"

#include <math.h>
#include "glm/glm.hpp"

SpriteAnimationComponent::SpriteAnimationComponent(std::unordered_map<std::string, SpriteAnimation> animations, std::string startAnimation) 
	: m_animations(animations), m_curAnimation(startAnimation), m_animationStartTime(Engine::GetInstance()->GetTime()->GetTime())
{
}

void SpriteAnimationComponent::PlayAnimation(std::string animation)
{
	m_curAnimation = animation;
	m_animationStartTime = Engine::GetInstance()->GetTime()->GetTime();
}

int SpriteAnimationComponent::GetCurrentFrame()
{
	float animationTime = Engine::GetInstance()->GetTime()->GetTime() - m_animationStartTime;
	SpriteAnimation curAnimation = m_animations[m_curAnimation];
	if (curAnimation.loop)
	{
		animationTime = fmod(animationTime, curAnimation.animationTime);
	}
	else if (animationTime > curAnimation.animationTime)
	{
		animationTime = curAnimation.animationTime;
	}

	int frame = curAnimation.startFrame + (int)glm::floor((curAnimation.endFrame - curAnimation.startFrame) * animationTime / curAnimation.animationTime);
	return frame;
}
