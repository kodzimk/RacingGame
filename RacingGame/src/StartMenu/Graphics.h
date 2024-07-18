#pragma once
#include<d3d11.h>
#include<wrl.h>
#define IMGUI_DEFINE_MATH_OPERATORS
#include"..\imgui\imgui.h"


using namespace Microsoft::WRL;

class Graphics
{
public:
	Graphics(HWND hwnd);
	~Graphics();

	void Clear(float red, float green, float blue) noexcept
	{
		const float color[] = { red,green,blue,1.0f };
		pContext->ClearRenderTargetView(pRenderTarget.Get(), color);
	}
	void Update(HWND hwnd);
	void End() {
		pSwapChain->Present(1u, 0);
	}
public:
	ComPtr<ID3D11Device> pDevice;
	ComPtr<IDXGISwapChain> pSwapChain;
	ComPtr<ID3D11DeviceContext> pContext;
	ComPtr<ID3D11RenderTargetView> pRenderTarget;
	ComPtr<ID3D11ShaderResourceView> my_texture;
	int ecode = -1;
private:
	 ImGuiIO* io = nullptr;
	 bool exit = false;
	 int my_image_width = 0;
	 int my_image_height = 0;

};

