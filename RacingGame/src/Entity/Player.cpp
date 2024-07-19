#include "Player.h"
#include<string>
#include<d3dcompiler.h>

DWORD indices[] =
{
	0, 1, 2, //FRONT
	0, 2, 3, //FRONT
	4, 7, 6, //BACK 
	4, 6, 5, //BACK
	3, 2, 6, //RIGHT SIDE
	3, 6, 7, //RIGHT SIDE
	4, 5, 1, //LEFT SIDE
	4, 1, 0, //LEFT SIDE
	1, 5, 6, //TOP
	1, 6, 2, //TOP
	0, 3, 7, //BOTTOM
	0, 7, 4, //BOTTOM
};

Player::Player(ID3D11Device* pDevice, LPCWSTR vertexDir, LPCWSTR pixelDir, float depthZ)
{
	vertices[0] = { -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f }; //FRONT Bottom Left   - [0]
	vertices[1] = { -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f }; //FRONT Top Left      - [1]
	vertices[2] = { 0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 1.0f }; //FRONT Top Right     - [2]
	vertices[3] = { 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f }; //FRONT Bottom Right   - [3]
	vertices[4] = { -0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 1.0f }; //BACK Bottom Left   - [4]
	vertices[5] = { -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f }; //BACK Top Left      - [5]
	vertices[6] = { 0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f }; //BACK Top Right     - [6]
	vertices[7] = { 0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f }; //BACK Bottom Right   - [7]

	UINT size = (UINT)std::size(indices);
	this->SetRotation(90.f, 0.0f, 0.0f);
	this->SetPosition(1.0f, -1.0f, -1.f);
}
Player::Player()
{
	this->SetRotation(0.0f, 0.0f, 0.0f);
	this->SetPosition(0.0f, 0.0f, 0.0f);
}
Player::~Player()
{
}

void Player::Draw(ID3D11DeviceContext* pContext, ID3D11Device* pDevice, DirectX::XMMATRIX matrix)
{

	UINT size = (UINT)std::size(indices);

	D3D11_BUFFER_DESC bd = {};
	bd.ByteWidth = sizeof(vertices);
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.StructureByteStride = sizeof(Vertex);

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices;

	pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer);

	D3D11_BUFFER_DESC ibd = {};
	ibd.ByteWidth = sizeof(indices);
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.StructureByteStride = sizeof(DWORD);

	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = indices;

	pDevice->CreateBuffer(&ibd, &isd, &pIndexBuffer);


	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	D3DReadFileToBlob(L"PPixel_Shader.cso", &pBlob);

	pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);

	D3DReadFileToBlob(L"PVertex_Shader.cso", &pBlob);

	pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader);

	const D3D11_INPUT_ELEMENT_DESC ied[] = {
		{"Pos",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"TEXCOORD",0,	DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"ALPHA",0,	DXGI_FORMAT_R32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0}
	};

	pDevice->CreateInputLayout(ied, (UINT)std::size(ied), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pInputLayout);

	const UINT stride = sizeof(Vertex);
	const UINT offset = 0;

	pContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
	pContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);
	pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);
	pContext->IASetInputLayout(pInputLayout.Get());
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	pContext->DrawIndexed(size, 0u, 0u);
}

void Player::UpdateWorldMatrix()
{
	this->worldMatrix = XMMatrixRotationRollPitchYaw(this->rot.x, this->rot.y, this->rot.z) * XMMatrixTranslation(this->pos.x, this->pos.y, this->pos.z);
	XMMATRIX vecRotationMatrix = XMMatrixRotationRollPitchYaw(this->rot.x, this->rot.y, 0.0f);

	this->vec_forward = XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
	this->vec_backward = XMVector3TransformCoord(this->DEFAULT_BACKWARD_VECTOR, vecRotationMatrix);
	this->vec_left = XMVector3TransformCoord(this->DEFAULT_LEFT_VECTOR, vecRotationMatrix);
	this->vec_right = XMVector3TransformCoord(this->DEFAULT_RIGHT_VECTOR, vecRotationMatrix);

}


const XMVECTOR& Player::GetPositionVector() const
{
	return this->posVector;
}

const XMFLOAT3& Player::GetPositionFloat3() const
{
	return this->pos;
}

const XMVECTOR& Player::GetRotationVector() const
{
	return this->rotVector;
}

const XMFLOAT3& Player::GetRotationFloat3() const
{
	return this->rot;
}

void Player::SetPosition(const XMVECTOR& pos)
{
	XMStoreFloat3(&this->pos, pos);
	this->posVector = pos;
	this->UpdateWorldMatrix();
}

void Player::SetPosition(const XMFLOAT3& pos)
{
	this->pos = pos;
	this->posVector = XMLoadFloat3(&this->pos);
	this->UpdateWorldMatrix();
}

void Player::SetPosition(float x, float y, float z)
{
	this->pos = XMFLOAT3(x, y, z);
	this->posVector = XMLoadFloat3(&this->pos);
	this->UpdateWorldMatrix();
}

void Player::AdjustPosition(const XMVECTOR& pos)
{
	this->posVector += pos;
	XMStoreFloat3(&this->pos, this->posVector);
	this->UpdateWorldMatrix();
}

void Player::AdjustPosition(const XMFLOAT3& pos)
{
	this->pos.x += pos.y;
	this->pos.y += pos.y;
	this->pos.z += pos.z;
	this->posVector = XMLoadFloat3(&this->pos);
	this->UpdateWorldMatrix();
}

void Player::AdjustPosition(float x, float y, float z)
{
	this->pos.x += x;
	this->pos.y += y;
	this->pos.z += z;
	this->posVector = XMLoadFloat3(&this->pos);
	this->UpdateWorldMatrix();
}

void Player::SetRotation(const XMVECTOR& rot)
{
	this->rotVector = rot;
	XMStoreFloat3(&this->rot, rot);
	this->UpdateWorldMatrix();
}

void Player::SetRotation(const XMFLOAT3& rot)
{
	this->rot = rot;
	this->rotVector = XMLoadFloat3(&this->rot);
	this->UpdateWorldMatrix();
}

void Player::SetRotation(float x, float y, float z)
{
	this->rot = XMFLOAT3(x, y, z);
	this->rotVector = XMLoadFloat3(&this->rot);
	this->UpdateWorldMatrix();
}

void Player::AdjustRotation(const XMVECTOR& rot)
{
	this->rotVector += rot;
	XMStoreFloat3(&this->rot, this->rotVector);
	this->UpdateWorldMatrix();
}

void Player::AdjustRotation(const XMFLOAT3& rot)
{
	this->rot.x += rot.x;
	this->rot.y += rot.y;
	this->rot.z += rot.z;
	this->rotVector = XMLoadFloat3(&this->rot);
	this->UpdateWorldMatrix();
}

void Player::AdjustRotation(float x, float y, float z)
{
	this->rot.x += x;
	this->rot.y += y;
	this->rot.z += z;
	this->rotVector = XMLoadFloat3(&this->rot);
	this->UpdateWorldMatrix();
}
void Player::SetLookAtPos(XMFLOAT3 lookAtPos)
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
const XMVECTOR& Player::GetForwardVector()
{
	return this->vec_forward;
}
const XMVECTOR& Player::GetRightVector()
{
	return this->vec_right;
}
const XMVECTOR& Player::GetBackwardVector()
{
	return this->vec_backward;
}
const XMVECTOR& Player::GetLeftVector()
{
	return this->vec_left;
}