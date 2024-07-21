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


	D3D11_TEXTURE2D_DESC dtd = {};
	dtd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dtd.ArraySize = 1;
	dtd.MipLevels = 1;
	dtd.CPUAccessFlags = 0;
	dtd.MiscFlags = 0;
	dtd.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	dtd.Usage = D3D11_USAGE_DEFAULT;
	dtd.SampleDesc.Count = 1;
	dtd.SampleDesc.Quality = 0;
	dtd.Height = 600;
	dtd.Width = 800;


	pDevice->CreateTexture2D(&dtd, nullptr, pDepthBuffer.GetAddressOf());
	pDevice->CreateDepthStencilView(pDepthBuffer.Get(), nullptr, pDepthView.GetAddressOf());

	pContext->OMSetRenderTargets(1, pRenderTarget.GetAddressOf(), pDepthView.Get());

	D3D11_DEPTH_STENCIL_DESC dt = {};
	dt.DepthFunc = D3D11_COMPARISON_LESS;
	dt.DepthEnable = true;
	dt.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;

	pDevice->CreateDepthStencilState(&dt, depthState.GetAddressOf());

	D3D11_VIEWPORT vt;
	vt.TopLeftX = 0;
	vt.TopLeftY = 0;
	vt.Width = 1024;
	vt.Height = 768;
	vt.MinDepth = 0;
	vt.MaxDepth = 1;

	pContext->RSSetViewports(1u, &vt); 

	camera.SetPosition(1.3f, 1.5f, -4.0f);
	camera.SetRotation(0.5f, 0.0f, 0.0f);
	camera.SetProjection(90.f, static_cast<float>(1024) / static_cast<float>(768), 0.1f, 1000.f);

	player = std::make_unique<Player>("../RacingGame/res/objects/Nissan.obj",L"../RacingGame/res/image/Nissan.png",pDevice.Get(), pContext.Get(), L"PVertex_Shader.cso", L"PPixel_Shader.cso", 1.0f);
	player2 = std::make_unique<Player>("../RacingGame/res/objects/Formula_Track.obj", L"../RacingGame/res/image/Asphalt.jpg", pDevice.Get(), pContext.Get(), L"PVertex_Shader.cso", L"PPixel_Shader.cso", 1.0f);
	player2->SetPosition(0.0f, -1.0f, 10.0f);
}	
LevelGraphics::~LevelGraphics()
{
	
}

void LevelGraphics::Update(HWND hwnd)
{
	Clear(1.0f, 1.0f, 0.0f);
	pContext->OMSetDepthStencilState(depthState.Get(), 0);

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

	matrix = worldMatrix * player2->worldMatrix * camera.GetViewMatrix() * camera.GetProjectionMatrix();
	matrix = XMMatrixTranspose(matrix);

	D3D11_BUFFER_DESC Cbd2 = {};
	Cbd2.ByteWidth = sizeof(matrix);
	Cbd2.Usage = D3D11_USAGE_DYNAMIC;
	Cbd2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	Cbd2.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	Cbd2.MiscFlags = 0u;
	Cbd2.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA csd2 = {};
	csd2.pSysMem = &matrix;

	pDevice->CreateBuffer(&Cbd2, &csd2, player2->pConstBuffer.GetAddressOf());
	pContext->VSSetConstantBuffers(0u, 1u, player2->pConstBuffer.GetAddressOf());
	player2->Draw(pContext.Get(), pDevice.Get(), matrix);
	End();
}
