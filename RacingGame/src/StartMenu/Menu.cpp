#include "Menu.h"
//#include"..\imgui\imgui.h"
//#include"..\imgui\imgui_impl_dx11.h"
//#include"..\imgui\imgui_impl_win32.h"
//
//extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND window, UINT msg, WPARAM wParam, LPARAM lParam);
//
//LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
//{
//	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
//		return true;
//
//	switch (msg)
//	{
//	case WM_DESTROY:
//		PostQuitMessage(0);
//		break;
//	}
//
//	return DefWindowProcW(hwnd, msg, wParam, lParam);
//}

Menu::Menu(int width, int height, HINSTANCE hInstance)
{
	name = "Race";
	WNDCLASSEX wnd;
	wnd.cbClsExtra = 0;
	wnd.hInstance = hInstance;
	wnd.cbSize = sizeof(WNDCLASSEX);
	wnd.lpszClassName = name.c_str();
	wnd.style = CS_OWNDC;
	wnd.lpfnWndProc = DefWindowProc;
	wnd.cbWndExtra = 0;
	wnd.hIcon = nullptr;
	wnd.hCursor = nullptr;
	wnd.hbrBackground = nullptr;
	wnd.hIconSm = nullptr;
	wnd.lpszMenuName = NULL;

	RegisterClassEx(&wnd);

	hwnd = CreateWindowEx(0, name.c_str(), "Racing Game", WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, width, height, nullptr, nullptr, hInstance, this);

	SetWindowLong(hwnd, GWL_STYLE, 0);
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

		gfx->Update(hwnd);
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return {};
}
