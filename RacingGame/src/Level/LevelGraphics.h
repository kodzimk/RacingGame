#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS
#include"..\imgui\imgui.h"
#include"..\System\Camera.h"
#include<d3d11.h>
#include<wrl.h>
#include"..\Entity\Player.h"
#include"..\Entity\GameObject.h"


class LevelGraphics
{
public:
	LevelGraphics(HWND hwnd);
	LevelGraphics() = default;
	~LevelGraphics();

	void Clear(float red, float green, float blue) noexcept
	{
		const float color[] = { red,green,blue,1.0f };
		pContext->ClearRenderTargetView(pRenderTarget.Get(), color);
		pContext->ClearDepthStencilView(pDepthView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
	void Update(HWND hwnd);
	void End() {
		pSwapChain->Present(1u, 0);
	}
public:
	Camera camera;
	std::unique_ptr<Player> player;
	std::unique_ptr<GameObject> map;
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRenderTarget;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDepthView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthState;
};
