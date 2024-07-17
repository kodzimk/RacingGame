#include "Windows.h"

Window::Window(int width, int height, HINSTANCE hInstance)
{
	this->hInstance = hInstance;
	name = "Race";
	WNDCLASSEX wnd;
	wnd.cbClsExtra = 0;
	wnd.hInstance = hInstance;
	wnd.cbSize = sizeof(WNDCLASSEX);
	wnd.lpszClassName = name.c_str();
	wnd.style = 0;
	wnd.lpfnWndProc = HandleMsgSetup;
	wnd.cbWndExtra = 0;
	wnd.hIcon = nullptr;
	wnd.hCursor = nullptr;
	wnd.hbrBackground = nullptr;
	wnd.hIconSm = nullptr;
	wnd.lpszMenuName = NULL;

	RegisterClassEx(&wnd);

	hwnd = CreateWindowEx(0, name.c_str(), "Racing Game", 0, 0, 0, width, height, nullptr, nullptr, hInstance, this);

	ShowWindow(hwnd, SW_SHOW);
}

Window::~Window()
{
	DestroyWindow(hwnd);
}

LRESULT Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{

	if (msg == WM_NCCREATE)
	{
		// extract ptr to window class from creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		// set WinAPI-managed user data to store ptr to window instance
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		// set message proc to normal (non-setup) handler now that setup is finished
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
		// forward message to window instance handler
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}
	// if we get a message before the WM_NCCREATE message, handle with default handler
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	// forward message to window instance handler
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		break;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}