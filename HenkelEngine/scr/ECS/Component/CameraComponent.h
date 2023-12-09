#pragma once

#include "glm\glm.hpp"

class CameraComponent
{
public:
	CameraComponent();

	void SetZoom(float zoom);
	float GetZoom() const;

	bool IsActiveCamera() const;
	void MakeCameraActive();
	void MakeCameraInactive();

	void DrawDebugPanel();

	void SetOffset(glm::vec2 offset);
	glm::vec2 GetOffset();	
	
	void SetDeadZone(glm::vec2 deadZone);
	glm::vec2 GetDeadZone();	
	
	void SetDamping(glm::vec2 damping);
	glm::vec2 GetDamping();

	bool debugLines;

private:

	float m_zoom;
	bool m_isActiveCamera;

	glm::vec2 m_offset;
	glm::vec2 m_deadZone;

	glm::vec2 m_damping;

};

