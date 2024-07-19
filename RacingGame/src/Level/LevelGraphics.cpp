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

	player = std::make_unique<Player>("../RacingGame/res/objects/Nissan.obj",pDevice.Get(),pContext.Get(), L"PVertex_Shader.cso", L"PPixel_Shader.cso", 1.0f);
}	
LevelGraphics::~LevelGraphics()
{
	
}

void LevelGraphics::Update(HWND hwnd)
{
	Clear(1.0f, 1.0f, 0.0f);
	XMMATRIX worldMatrix = XMMatrixIdentity();
	XMMATRIX matrix =  worldMatrix * player->worldMatrix * camera.GetViewMatrix() * camera.GetProjectionMatrix();
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

	pDevice->CreateBuffer(&Cbd, &csd, player->pConstBuffer.GetAddressOf());
	pContext->VSSetConstantBuffers(0u, 1u, player->pConstBuffer.GetAddressOf());

	player->Draw(pContext.Get(), pDevice.Get(), matrix);
	End();
}
