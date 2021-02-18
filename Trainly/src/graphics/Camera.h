#pragma once

#include "../stdafx.h"

#include <cmath>

class Camera
{
public:
	Camera();

	void Init(float fov, float aspectRatio, float nearZ, float farZ);

	DirectX::SimpleMath::Vector3 GetForwardVector() { return m_vecForward; }
	DirectX::SimpleMath::Vector3 GetRightVector() { return m_vecRight; }
	DirectX::SimpleMath::Vector3 GetBackwardVector() { return m_vecBackward; }
	DirectX::SimpleMath::Vector3 GetLeftVector() { return m_vecLeft; }

	DirectX::SimpleMath::Vector3 GetUpVector() { return m_vecUp; }
	DirectX::SimpleMath::Vector3 GetDownVector() { return m_vecDown; }

	inline DirectX::SimpleMath::Vector3& GetPosition() {
		return m_position;
	}

	inline void SetPosition(DirectX::SimpleMath::Vector3 vec) {
		m_position = vec;
		UpdateViewMatrix();
	}

	inline void AppendPosition(DirectX::SimpleMath::Vector3 vec) {
		m_position += vec;
		UpdateViewMatrix();
	}

	inline void AppendRotation(DirectX::SimpleMath::Vector3 vec) {
		m_rotation += vec;
		UpdateViewMatrix();
	}

	inline DirectX::SimpleMath::Vector3& GetRotation() {
		return m_rotation;
	}

	inline void SetRotation(DirectX::SimpleMath::Vector3 vec) {
		m_rotation = vec;
		UpdateViewMatrix();

	}

	inline DirectX::SimpleMath::Matrix& GetProjection() {
		return m_projectionMatrix;
	}

	inline DirectX::SimpleMath::Matrix& GetView() {
		return m_viewMatrix;
	}

	inline void SetLookTargetPosition(DirectX::SimpleMath::Vector3 lookPos) {
		if (lookPos == m_position)
			return;

		lookPos = m_position - lookPos;

		float pitch = 0.0f;
		if (lookPos.y != 0.0f)
		{
			float dist = sqrt(pow(lookPos.x, 2) + pow(lookPos.z, 2));
			pitch = atan(lookPos.y / dist);
		}

		float yaw = 0.0f;
		if (lookPos.x != 0.0f)
		{
			yaw = atan(lookPos.x / lookPos.z);
		}
		if (lookPos.z > 0)
			yaw += DirectX::XM_PI;



		this->SetRotation({ pitch, yaw, 0.0f });
	}

private:

	void UpdateViewMatrix();

	float fov;
	float nearZ;
	float farZ;

	DirectX::SimpleMath::Vector3 m_position;
	DirectX::SimpleMath::Vector3 m_rotation;
	DirectX::SimpleMath::Matrix m_viewMatrix;
	DirectX::SimpleMath::Matrix m_projectionMatrix;

	DirectX::SimpleMath::Vector3 DEFAULT_UP_VECTOR = { 0.0f, 1.0f, 0.0f };
	DirectX::SimpleMath::Vector3 DEFAULT_FORWARD_VECTOR = { 0.0f, 0.0f, 1.0f };
	DirectX::SimpleMath::Vector3 DEFAULT_BACKWARD_VECTOR = { 0.0f, 0.0f, -1.0f };
	DirectX::SimpleMath::Vector3 DEFAULT_LEFT_VECTOR = { -1.0f, 0.0f, 0.0f };
	DirectX::SimpleMath::Vector3 DEFAULT_RIGHT_VECTOR = { 1.0f, 0.0f, 0.0f };
	DirectX::SimpleMath::Vector3 DEFAULT_DOWN_VECTOR = { 0.0f,-1.0f, 0.0f };


	DirectX::SimpleMath::Vector3 m_vecForward;
	DirectX::SimpleMath::Vector3 m_vecLeft;
	DirectX::SimpleMath::Vector3 m_vecRight;
	DirectX::SimpleMath::Vector3 m_vecBackward;

	DirectX::SimpleMath::Vector3 m_vecUp;
	DirectX::SimpleMath::Vector3 m_vecDown;

public:
	void Resize(float w, float h);
	void SetFov(float angle);
private:
	float aspectRatio;
};

