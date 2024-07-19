#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS
#include"..\imgui\imgui.h"
#include"..\System\Camera.h"
#include<d3d11.h>
#include<wrl.h>


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
	}
	void Update(HWND hwnd);
	void End() {
		pSwapChain->Present(1u, 0);
	}
public:
	Camera camera;
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRenderTarget;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> my_texture;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;

};
