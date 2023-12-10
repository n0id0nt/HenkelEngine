#pragma once

#include "glm\glm.hpp"
#include "sol\sol.hpp"

class Camera;

class CameraComponent
{
public:
	CameraComponent();

	void SetZoom(float zoom);
	float GetZoom() const;

	bool IsActiveCamera() const;
	void MakeCameraActive();
	void MakeCameraActiveFromCamera(Camera* camera);
	void MakeCameraInactive();
	void MakeCameraInactiveFromCamera(Camera* camera);

	void DrawDebugPanel();

	void SetOffset(glm::vec2 offset);
	glm::vec2 GetOffset();	
	
	void SetDeadZone(glm::vec2 deadZone);
	glm::vec2 GetDeadZone();	
	
	void SetDamping(glm::vec2 damping);
	glm::vec2 GetDamping();

	void SetAngle(float angle);
	float GetAngle();

	void ForcePosition();
	void SetForcePosition(bool value);
	bool IsPositionForced();

	void AddTrauma(float trauma);
	float GetTrauma();
	void SetTrauma(float trauma);
	void ResetTrauma();
	float GetTraumaTime();
	float GetMaxOffset();
	float GetMaxAngle();

	bool debugLines;

	static void LUABind(sol::state& lua);

private:

	float m_zoom;
	bool m_isActiveCamera;

	glm::vec2 m_offset;
	glm::vec2 m_deadZone;

	glm::vec2 m_damping;

	bool m_forcePosition;

	float m_traumaTime;
	float m_maxOffset;
	float m_maxAngle;
	float m_trauma;

	float m_angle;
};

