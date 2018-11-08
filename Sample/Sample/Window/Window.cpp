#include "Window.h"
#include "../Application/Application.h"
#include <Windows.h>
#include <tchar.h>

// コンストラクタ　
Window::Window(void ** handle) : 
	instance(nullptr), name(nullptr)
{
	Create(handle);
}

// デストラクタ
Window::~Window()
{
	UnregisterClass(name, (HINSTANCE)(instance));
}

// タイトル名の変更
void Window::ChangeTitle(void ** handle, const wchar_t * title)
{
	auto hr = SetWindowText(reinterpret_cast<HWND>(*handle), title);
	if (hr == 0)
	{
		OutputDebugString(_T("\nタイトル名の変更：失敗\n"));
	}
}

// ウィンドウの生成
void Window::Create(void ** handle)
{
	WNDCLASSEX wnd{};
	wnd.cbClsExtra    = 0;
	wnd.cbSize        = sizeof(WNDCLASSEX);
	wnd.cbWndExtra    = 0;
	wnd.hbrBackground = CreateSolidBrush(0x000000);
	wnd.hCursor       = LoadCursor(nullptr, IDC_ARROW);
	wnd.hIcon         = LoadIcon(nullptr, IDI_APPLICATION);
	wnd.hIconSm       = LoadIcon(GetModuleHandle(0), IDI_APPLICATION);
	wnd.hInstance     = GetModuleHandle(0);
	wnd.lpfnWndProc   = (WNDPROC)Application::WindowProc;
	wnd.lpszClassName = _T("DirectX12");
	wnd.lpszMenuName  = _T("DirectX12");
	wnd.style         = CS_HREDRAW | CS_VREDRAW;
	RegisterClassEx(&wnd);

	RECT rect{};
	rect.bottom = static_cast<long>(Application::GetHeight());
	rect.left   = 0;
	rect.right  = static_cast<long>(Application::GetWidth());
	rect.top    = 0;
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	*handle = CreateWindow(wnd.lpszClassName, _T("おかもん"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		(rect.right - rect.left), (rect.bottom - rect.top), nullptr, nullptr, wnd.hInstance, nullptr);
	if (*handle == nullptr)
	{
		return;
	}

	instance = wnd.hInstance;
	name     = wnd.lpszClassName;
	
	ShowWindow(reinterpret_cast<HWND>(*handle), SW_SHOW);
}
