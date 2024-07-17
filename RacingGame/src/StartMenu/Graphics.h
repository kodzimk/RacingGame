#pragma once
#include<d3d11.h>
#include<wrl.h>

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
	void Update();
	void End() {
		pSwapChain->Present(1u, 0);
	}
public:
	ComPtr<ID3D11Device> pDevice;
	ComPtr<IDXGISwapChain> pSwapChain;
	ComPtr<ID3D11DeviceContext> pContext;
	ComPtr<ID3D11RenderTargetView> pRenderTarget;
};

