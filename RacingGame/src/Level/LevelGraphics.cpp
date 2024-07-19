#include "LevelGraphics.h"
#include<d3dcompiler.h>

LevelGraphics::LevelGraphics(HWND hwnd)
{
	DXGI_SWAP_CHAIN_DESC cd = {};
	cd.BufferDesc.Width = 800;
	cd.BufferDesc.Height = 600;
	cd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	cd.BufferDesc.RefreshRate.Denominator = 0;
	cd.BufferDesc.RefreshRate.Numerator = 0;
	cd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	cd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	cd.BufferCount = 1;
	cd.Windowed = true;
	cd.SampleDesc.Count = 1;
	cd.SampleDesc.Quality = 0;
	cd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	cd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	cd.OutputWindow = hwnd;
	cd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;


	D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&cd,
		&pSwapChain,
		&pDevice,
		nullptr,
		&pContext
	);

	Microsoft::WRL::ComPtr<ID3D11Resource> backBuffer;

	pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
	pDevice->CreateRenderTargetView(backBuffer.Get(), NULL, pRenderTarget.GetAddressOf());

	pContext->OMSetRenderTargets(1u, pRenderTarget.GetAddressOf(), nullptr);

	D3D11_VIEWPORT vt;
	vt.TopLeftX = 0;
	vt.TopLeftY = 0;
	vt.Width = 1024;
	vt.Height = 768;
	vt.MinDepth = 0;
	vt.MaxDepth = 1;

	pContext->RSSetViewports(1u, &vt); 

	camera.SetPosition(0.0f, 0.0f, -2.0f);
	camera.SetProjection(90.f, static_cast<float>(1024) / static_cast<float>(768), 0.1f, 1000.f);
}

	

LevelGraphics::~LevelGraphics()
{
	
}

struct Vertex {
	float x;
	float y;
	float z;
	float r;
	float g;
	float a;
}vertices[8];

void LevelGraphics::Update(HWND hwnd)
{
	Clear(1.0f, 1.0f, 0.0f);

	vertices[0] = { -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f }; //FRONT Bottom Left   - [0]
	vertices[1] = { -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f }; //FRONT Top Left      - [1]
	vertices[2] = { 0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 1.0f }; //FRONT Top Right     - [2]
	vertices[3] = { 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f }; //FRONT Bottom Right   - [3]
	vertices[4] = { -0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 1.0f }; //BACK Bottom Left   - [4]
	vertices[5] = { -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f }; //BACK Top Left      - [5]
	vertices[6] = { 0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f }; //BACK Top Right     - [6]
	vertices[7] = { 0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f }; //BACK Bottom Right   - [7]

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

	XMMATRIX worldMatrix = XMMatrixIdentity();

	XMMATRIX matrix = worldMatrix * camera.GetViewMatrix() * camera.GetProjectionMatrix();
	matrix = XMMatrixTranspose(matrix);

	D3D11_BUFFER_DESC Cbd = {};
	Cbd.ByteWidth = sizeof(matrix);
	Cbd.Usage = D3D11_USAGE_DYNAMIC;
	Cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	Cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	Cbd.MiscFlags = 0u;
	Cbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA csd = {};
	csd.pSysMem = &matrix;

	const UINT stride = sizeof(Vertex);
	const UINT offset = 0;

	pDevice->CreateBuffer(&Cbd, &csd, pConstantBuffer.GetAddressOf());
    pContext->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());
	pContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
	pContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);
	pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);
	pContext->IASetInputLayout(pInputLayout.Get());
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	pContext->DrawIndexed(size, 0u, 0u);

	End();
}
