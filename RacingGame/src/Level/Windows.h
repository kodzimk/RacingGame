#pragma once
#include<d3d11.h>
#include<dxgi.h>
#include<Windows.h>
#include<string>
#include<memory>
#include<optional>
#include"..\System\Keyboard.h"
#include"..\System\Mouse.h"
#include"LevelGraphics.h"

class LevelGraphics;

class Window
{
public:
	Window(int width, int height, HINSTANCE hInstance);
	Window() = default;
	~Window();
	std::optional<int>  ProccesMessages();
	void InputHandler();
	void Update();

public:
	const HWND getWindow() { return hwnd; };
	const HINSTANCE getInstance() { return hInstance; };
private:
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT CALLBACK HandleMsg(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
public:
	std::unique_ptr<LevelGraphics> gfx;
	 KeyboardClass keyboard;
	 MouseClass mouse;
	 int ecode;
private:
	HINSTANCE hInstance;
	HWND hwnd;
};


