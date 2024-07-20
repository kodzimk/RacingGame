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

	XMMATRIX pitchRot = XMMatrixRotationRollPitchYaw(0.0f, rot.y, rot.z);
	front_vec = XMVector3TransformCoord(FORWARD_VECTOR, pitchRot);
	back_vec = XMVector3TransformCoord(BACWARD_VECTOR, pitchRot);
	left_vec = XMVector3TransformCoord(LEFT_VECTOR, pitchRot);
	right_vec = XMVector3TransformCoord(RIGHT_VECTOR, pitchRot);

}

void Camera::SetLookAtPos(XMFLOAT3 lookAtPos)
{
	//Verify that look at pos is not the same as cam pos. They cannot be the same as that wouldn't make sense and would result in undefined behavior.
	if (lookAtPos.x == this->pos.x && lookAtPos.y == this->pos.y && lookAtPos.z == this->pos.z)
		return;

	lookAtPos.x = this->pos.x - lookAtPos.x;
	lookAtPos.y = this->pos.y - lookAtPos.y;
	lookAtPos.z = this->pos.z - lookAtPos.z;

	float pitch = 0.0f;
	if (lookAtPos.y != 0.0f)
	{
		const float distance = sqrt(lookAtPos.x * lookAtPos.x + lookAtPos.z * lookAtPos.z);
		pitch = atan(lookAtPos.y / distance);
	}

	float yaw = 0.0f;
	if (lookAtPos.x != 0.0f)
	{
		yaw = atan(lookAtPos.x / lookAtPos.z);
	}
	if (lookAtPos.z > 0)
		yaw += XM_PI;

	this->SetRotation(pitch, yaw, 0.0f);
}
