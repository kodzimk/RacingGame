#pragma once
#include<d3d11.h>
#include<dxgi.h>
#include<Windows.h>
#include<string>
#include<memory>
#include<optional>
#include"..\System\Keyboard.h"
#include"..\System\Mouse.h"

class Window
{
public:
	Window(int width, int height, HINSTANCE hInstance);
	Window() = default;
	~Window();
	std::optional<int>  ProccesMessages();
	void InputHandler();

public:
	const std::string& getWindowName() const { return windowName; };
	const HWND getWindow() { return hwnd; };
	const HINSTANCE getInstance() { return hInstance; };
private:
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT CALLBACK HandleMsg(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
public:
	 KeyboardClass keyboard;
	 MouseClass mouse;
	 int ecode;
private:
	HINSTANCE hInstance;
	HWND hwnd;
	std::string windowName;
	std::string name;
};


