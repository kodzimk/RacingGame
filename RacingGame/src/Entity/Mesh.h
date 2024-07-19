#pragma once
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <directxmath.h>
#include<memory>
#include<SpriteBatch.h>
#include<SpriteFont.h>
#include<d3d11.h>
#include<wrl.h>
#include<WICTextureLoader.h>
#include<vector>
#include<DirectXMath.h>

using namespace DirectX;

struct Vertex {
	float x;
	float y;
	float z;
	float r;
	float g;
	float a;
};

class Mesh
{
public:
	Mesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::vector<Vertex>& vertices, std::vector<DWORD>& indices);
	Mesh(const Mesh& mesh);
	void Draw();
private:
	UINT size;
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexbuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexbuffer;
	ID3D11DeviceContext* deviceContext;
};