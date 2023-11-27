#pragma once
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

private:

	float m_zoom;
	bool m_isActiveCamera;
};

