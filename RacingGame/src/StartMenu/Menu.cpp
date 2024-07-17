#include "Menu.h"

LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

Menu::Menu(int width, int height, HINSTANCE hInstance)
{
	name = "Race";
	WNDCLASSEX wnd;
	wnd.cbClsExtra = 0;
	wnd.hInstance = hInstance;
	wnd.cbSize = sizeof(WNDCLASSEX);
	wnd.lpszClassName = name.c_str();
	wnd.style = CS_OWNDC;
	wnd.lpfnWndProc = WndProc;
	wnd.cbWndExtra = 0;
	wnd.hIcon = nullptr;
	wnd.hCursor = nullptr;
	wnd.hbrBackground = nullptr;
	wnd.hIconSm = nullptr;
	wnd.lpszMenuName = NULL;

	RegisterClassEx(&wnd);

	hwnd = CreateWindowEx(0, name.c_str(), "Racing Game", WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, width, height, nullptr, nullptr, hInstance, this);

	ShowWindow(hwnd, SW_SHOW);

	gfx = std::make_unique<Graphics>(hwnd);
}

Menu::~Menu()
{
	DestroyWindow(hwnd);
}

std::optional<int> Menu::ProccesMessages()
{
	MSG msg;

	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			delete this;
			return msg.wParam;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return {};
}
