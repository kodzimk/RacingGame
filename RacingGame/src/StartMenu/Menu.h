#pragma once
#include<d3d11.h>
#include<dxgi.h>
#include<Windows.h>
#include<optional>
#include<string>
#include<memory>
#include"Graphics.h"

class Menu
{
public:
	Menu(int width, int height, HINSTANCE hInstance);
	~Menu();
	std::optional<int>  ProccesMessages();
public:
	const std::string& getWindowName() const { return windowName; };
	const HWND getWindow() { return hwnd; };
	const HINSTANCE getInstance() { return hInstance; };
public:
	std::unique_ptr<Graphics> gfx;
private:
	HINSTANCE hInstance;
	HWND hwnd;
	std::string windowName;
	std::string name;
};

