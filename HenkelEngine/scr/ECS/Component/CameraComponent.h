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
	
	void SetSoftZone(glm::vec2 softZone);
	glm::vec2 GetSoftZone();
	
	void SetSoftDamping(glm::vec2 softDamping);
	glm::vec2 GetSoftDamping();
	
	void SetHardDamping(glm::vec2 hardDamping);
	glm::vec2 GetHardDamping();

	bool debugLines;
private:

	float m_zoom;
	bool m_isActiveCamera;

	glm::vec2 m_offset;
	glm::vec2 m_deadZone;
	glm::vec2 m_softZone;

	glm::vec2 m_softDamping;
	glm::vec2 m_hardDamping;

};

