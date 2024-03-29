#include "Camera.h"

using namespace DirectX::SimpleMath;

Camera::Camera()
{
	m_position = { 0.0f, 0.0f, 0.0f };
	m_rotation = { 0.0f, 0.0f, 0.0f };

	UpdateViewMatrix();
}

void Camera::Init(float fov, float aspectRatio, float nearZ, float farZ)
{
	float fovRadians = (fov / 360.0f) * DirectX::XM_2PI;

	this->fov = fovRadians;
	this->aspectRatio = aspectRatio;
	this->nearZ = nearZ;
	this->farZ = farZ;

	m_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fovRadians, this->aspectRatio, nearZ, farZ);
	this->UpdateViewMatrix();
}

void Camera::UpdateViewMatrix()
{
	Matrix camRotMat = DirectX::XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z);
	Vector3 camTarget = DirectX::XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, camRotMat);

	camTarget += m_position;

	Vector3 upDir = Vector3::Transform(this->DEFAULT_UP_VECTOR, camRotMat);

	m_viewMatrix = DirectX::XMMatrixLookAtLH(m_position, camTarget, upDir);

	Matrix vecRotMat = DirectX::XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z);

	this->m_vecForward = XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, vecRotMat);
	this->m_vecBackward = XMVector3TransformCoord(this->DEFAULT_BACKWARD_VECTOR, vecRotMat);
	this->m_vecLeft = XMVector3TransformCoord(this->DEFAULT_LEFT_VECTOR, vecRotMat);
	this->m_vecRight = XMVector3TransformCoord(this->DEFAULT_RIGHT_VECTOR, vecRotMat);
	this->m_vecUp = XMVector3TransformCoord(this->DEFAULT_UP_VECTOR, vecRotMat);
	this->m_vecDown = XMVector3TransformCoord(this->DEFAULT_DOWN_VECTOR, vecRotMat);
}

void Camera::Resize(float w, float h)
{
	std::cout << "Resizing to: " << w << " x " << h << std::endl;
	aspectRatio = w / h * 1.0f;

	m_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(this->fov, aspectRatio, nearZ, farZ);
	this->UpdateViewMatrix();
}

void Camera::SetFov(float angle)
{
	this->fov = (angle / 360.0f) * DirectX::XM_2PI;
	m_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(this->fov, aspectRatio, nearZ, farZ);
	this->UpdateViewMatrix();
}

