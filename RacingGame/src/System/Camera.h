#pragma once
#include <DirectXMath.h>
using namespace DirectX;

class Camera
{
public:
	Camera();
	void SetProjection(float fovDegrees, float aspectRotio, float nearZ, float farZ);
	void AdjustPosition(const XMVECTOR& vec);
	void AdjustPosition(float x, float y, float z);
	void SetPosition(const XMVECTOR& vec);
	void SetPosition(float x, float y, float z);
	void AdjustRotation(const XMVECTOR& vec);
	void AdjustRotation(float x, float y, float z);
	void SetRotation(const XMVECTOR& vec);
	void SetRotation(float x, float y, float z);

	const XMVECTOR GetForwardVector() const;
	const XMVECTOR GetBackWardVector() const;
	const XMVECTOR GetLeftVector() const;
	const XMVECTOR GetRightVector() const;
	XMMATRIX GetViewMatrix();
	XMMATRIX GetProjectionMatrix();

private:
	void UpdateViewMatrix();

private:
	XMMATRIX projectionMatrix;
	XMMATRIX viewMatrix;
	XMVECTOR rotVector;
	XMVECTOR posVector;
	XMFLOAT3 rot;
	XMFLOAT3 pos;

	XMVECTOR front_vec;
	XMVECTOR back_vec;
	XMVECTOR left_vec;
	XMVECTOR right_vec;

	const XMVECTOR FORWARD_VECTOR = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const XMVECTOR BACWARD_VECTOR = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
	const XMVECTOR LEFT_VECTOR = XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
	const XMVECTOR RIGHT_VECTOR = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	const XMVECTOR UP_VECTOR = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

};