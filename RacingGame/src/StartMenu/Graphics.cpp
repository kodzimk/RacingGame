#include "Graphics.h"
#include"..\imgui\imgui_impl_dx11.h"
#include"..\imgui\imgui.h"
#include"..\imgui\imgui_impl_win32.h"

#define STB_IMAGE_IMPLEMENTATION
#include "..\Others\stbi_image.h"

// Simple helper function to load an image into a DX11 texture with common settings
bool LoadTextureFromFile(const char* filename, ID3D11ShaderResourceView** out_srv,
	int* out_width, int* out_height,ID3D11Device* pDevice)
{
	// Load from disk into a  raw RGBA buffer
	int image_width = 0;
	int image_height = 0;
	unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
	if (image_data == NULL)
		return false;

	// Create texture
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = image_width;
	desc.Height = image_height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;

	ID3D11Texture2D* pTexture = NULL;
	D3D11_SUBRESOURCE_DATA subResource;
	subResource.pSysMem = image_data;
	subResource.SysMemPitch = desc.Width * 4;
	subResource.SysMemSlicePitch = 0;
	pDevice->CreateTexture2D(&desc, &subResource, &pTexture);

	// Create texture view
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = desc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	pDevice->CreateShaderResourceView(pTexture, &srvDesc, out_srv);
	pTexture->Release();

	*out_width = image_width;
	*out_height = image_height;
	stbi_image_free(image_data);

	return true;
}

Graphics::Graphics(HWND hwnd)
{
	DXGI_SWAP_CHAIN_DESC cd = {};
	cd.BufferDesc.Width = 800;
	cd.BufferDesc.Height = 600;
	cd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	cd.BufferDesc.RefreshRate.Denominator= 0;
	cd.BufferDesc.RefreshRate.Numerator = 0;
	cd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	cd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	cd.BufferCount = 1;
	cd.Windowed = true;
	cd.SampleDesc.Count =1;
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
	vt.Width = 800;
	vt.Height = 600;
	vt.MinDepth = 0;
	vt.MaxDepth = 1;

	pContext->RSSetViewports(1u, &vt);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(pDevice.Get(), pContext.Get());
	ImGui::StyleColorsDark();

}

Graphics::~Graphics()
{
}

void Graphics::Update()
{
	Clear(1.0f, 1.0f, 1.0f);

	int my_image_width = 0;
	int my_image_height = 0;
	ID3D11ShaderResourceView* my_texture = NULL;
	bool ret = LoadTextureFromFile("res/image/Background.jpeg", &my_texture, &my_image_width, &my_image_height,pDevice.Get());
	IM_ASSERT(ret);

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Racing Game",&exit,ImGuiWindowFlags_NoResize |
		                    ImGuiWindowFlags_NoSavedSettings | 
		                     ImGuiWindowFlags_NoCollapse | 
		                      ImGuiWindowFlags_NoMove);

	ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::SetWindowSize(ImVec2(1024, 768));
	ImGui::Image((void*)my_texture, ImVec2(768, 384));
	ImGui::End();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	End();
}
