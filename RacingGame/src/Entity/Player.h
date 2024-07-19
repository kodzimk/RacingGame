#pragma once
#include<d3d11.h>
#include<wrl.h>
#include<DirectXMath.h>
#include"Mesh.h"

using namespace DirectX;

class Player
{
public:
	Player(const std::string& filePath, const wchar_t* fileName, ID3D11Device* pDevice,ID3D11DeviceContext* pContext,LPCWSTR vertexDir, LPCWSTR pixelDir, float depthZ);
	Player();
	~Player();

	//Tranformation 
public:
	const XMVECTOR& GetPositionVector() const;
	const XMFLOAT3& GetPositionFloat3() const;
	const XMVECTOR& GetRotationVector() const;
	const XMFLOAT3& GetRotationFloat3() const;

	void SetPosition(const XMVECTOR& pos);
	void SetPosition(const XMFLOAT3& pos);
	void SetPosition(float x, float y, float z);
	void AdjustPosition(const XMVECTOR& pos);
	void AdjustPosition(const XMFLOAT3& pos);
	void AdjustPosition(float x, float y, float z);
	void SetRotation(const XMVECTOR& rot);
	void SetRotation(const XMFLOAT3& rot);
	void SetRotation(float x, float y, float z);
	void AdjustRotation(const XMVECTOR& rot);
	void AdjustRotation(const XMFLOAT3& rot);
	void AdjustRotation(float x, float y, float z);
	void SetLookAtPos(XMFLOAT3 lookAtPos);
	const XMVECTOR& GetForwardVector();
	const XMVECTOR& GetRightVector();
	const XMVECTOR& GetBackwardVector();
	const XMVECTOR& GetLeftVector();
private:
	void UpdateWorldMatrix();
public:
	XMMATRIX worldMatrix = XMMatrixIdentity();

	XMVECTOR posVector;
	XMVECTOR rotVector;
	XMFLOAT3 pos;
	XMFLOAT3 rot;

	const XMVECTOR DEFAULT_FORWARD_VECTOR = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const XMVECTOR DEFAULT_UP_VECTOR = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const XMVECTOR DEFAULT_BACKWARD_VECTOR = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
	const XMVECTOR DEFAULT_LEFT_VECTOR = XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
	const XMVECTOR DEFAULT_RIGHT_VECTOR = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	XMVECTOR vec_forward;
	XMVECTOR vec_left;
	XMVECTOR vec_right;
	XMVECTOR vec_backward;

	//Draw Elements
public:
	void Draw(ID3D11DeviceContext* pContext, ID3D11Device* pDevice,DirectX::XMMATRIX matrix);
private:
	bool LoadModel(const std::string& filePath);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
public:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstBuffer;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampleState;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTexture;
	std::vector<Mesh> meshes;
private:
	UINT size = 0;
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
};