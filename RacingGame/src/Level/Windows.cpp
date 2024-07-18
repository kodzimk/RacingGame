#include "Windows.h"

Window::Window(int width, int height, HINSTANCE hInstance)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = this->hInstance;
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = TEXT("DirectxGame");
	wc.hIconSm = nullptr;

	RegisterClassEx(&wc);


	hwnd = CreateWindowEx(0, TEXT("DirectxGame"), TEXT("Racing Game"),
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, width, height,
		nullptr, nullptr, hInstance, this
	);

	if (hwnd == nullptr)
	{
		throw "hwnd problem";
	}

	ShowWindow(hwnd, SW_SHOW);

	static bool raw_input_init = false;

	if (raw_input_init == false)
	{
		RAWINPUTDEVICE rid;
		rid.usUsagePage = 0x01;
		rid.usUsage = 0x02;
		rid.dwFlags = 0;
		rid.hwndTarget = NULL;

		if (RegisterRawInputDevices(&rid, 1, sizeof(rid)) == FALSE)
		{
			throw "Failed to register raw input devices";
			return;
		}
	}
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
	case WM_CLOSE:
		PostQuitMessage(0);
		ecode = 2;
		break;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

std::optional<int> Window::ProccesMessages()
{
	MSG msg;

	while (PeekMessage(&msg, nullptr, 0, 0,PM_REMOVE))
	{
		if (msg.message == WM_CLOSE)
		{	
			return 2;
		}
		if (ecode == 2)
			return 2;

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return {};
}