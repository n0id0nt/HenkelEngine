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

std::string SpriteAnimationComponent::GetCurrentAnimation()
{
	return m_curAnimation;
}

bool SpriteAnimationComponent::IsAnimationPlaying()
{
	SpriteAnimation curAnimation = m_animations[m_curAnimation];
	float animationTime = Engine::GetInstance()->GetTime()->GetTime() - m_animationStartTime;
	return curAnimation.loop || animationTime <= curAnimation.animationTime;
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

void SpriteAnimationComponent::LUABind(sol::state& lua)
{
	lua.new_usertype<SpriteAnimationComponent>("spriteAnimation",
		"playAnimation", &SpriteAnimationComponent::PlayAnimation,
		"getCurrentAnimation", &SpriteAnimationComponent::GetCurrentAnimation,
		"isAnimationPlaying", &SpriteAnimationComponent::IsAnimationPlaying
	);
}
