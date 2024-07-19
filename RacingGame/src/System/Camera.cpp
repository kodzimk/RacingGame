#include "Camera.h"

Camera::Camera()
{
	rot = { 0.0f,0.0f,0.0f };
	rotVector = XMLoadFloat3(&rot);
	pos = { 0.0f,0.0f,-2.0f };
	posVector = XMLoadFloat3(&pos);
	UpdateViewMatrix();
}

void Camera::SetProjection(float fovDegrees, float aspectRotio, float nearZ, float farZ)
{
	float fovRadians = (fovDegrees / 360.f) * XM_2PI;
	projectionMatrix = XMMatrixPerspectiveFovLH(fovRadians, aspectRotio, nearZ, farZ);
}

void Camera::AdjustPosition(const XMVECTOR& vec)
{
	posVector += vec;
	XMStoreFloat3(&pos, posVector);
	UpdateViewMatrix();
}

void Camera::AdjustPosition(float x, float y, float z)
{
	pos.x += x;
	pos.y += y;
	pos.z += z;
	posVector = XMLoadFloat3(&pos);
	UpdateViewMatrix();
}

void Camera::SetPosition(const XMVECTOR& vec)
{
	XMStoreFloat3(&pos, vec);
	posVector = vec;
	UpdateViewMatrix();
}

void Camera::SetPosition(float x, float y, float z)
{
	pos.x = x;
	pos.y = y;
	pos.z = z;
	posVector = XMLoadFloat3(&pos);
	UpdateViewMatrix();
}

void Camera::AdjustRotation(const XMVECTOR& vec)
{
	rotVector += vec;
	XMStoreFloat3(&rot, rotVector);
	UpdateViewMatrix();
}

void Camera::AdjustRotation(float x, float y, float z)
{
	rot.x += x;
	rot.y += y;
	rot.z += z;
	rotVector = XMLoadFloat3(&rot);
	UpdateViewMatrix();
}

void Camera::SetRotation(const XMVECTOR& vec)
{
	XMStoreFloat3(&rot, vec);
	rotVector = vec;
	UpdateViewMatrix();
}

void Camera::SetRotation(float x, float y, float z)
{
	rot.x = x;
	rot.y = y;
	rot.z = z;
	rotVector = XMLoadFloat3(&rot);
	UpdateViewMatrix();
}

const XMVECTOR Camera::GetForwardVector() const
{
	return front_vec;
}

const XMVECTOR Camera::GetBackWardVector() const
{
	return back_vec;
}

const XMVECTOR Camera::GetLeftVector() const
{
	return left_vec;
}

const XMVECTOR Camera::GetRightVector() const
{
	return right_vec;
}

XMMATRIX Camera::GetViewMatrix()
{
	return viewMatrix;
}

XMMATRIX Camera::GetProjectionMatrix()
{
	return projectionMatrix;
}

void Camera::UpdateViewMatrix()
{
	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);

	XMVECTOR frontVector = XMVector3TransformCoord(FORWARD_VECTOR, rotationMatrix);

	frontVector += posVector;

	XMVECTOR upDir = XMVector3TransformCoord(UP_VECTOR, rotationMatrix);

	viewMatrix = XMMatrixLookAtLH(posVector, frontVector, upDir);

	XMMATRIX pitchRot = XMMatrixRotationRollPitchYaw(rot.x, rot.y, 0.0f);
	front_vec = XMVector3TransformCoord(FORWARD_VECTOR, pitchRot);
	back_vec = XMVector3TransformCoord(BACWARD_VECTOR, pitchRot);
	left_vec = XMVector3TransformCoord(LEFT_VECTOR, pitchRot);
	right_vec = XMVector3TransformCoord(RIGHT_VECTOR, pitchRot);

}
