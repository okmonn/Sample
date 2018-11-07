#include "Application.h"
#include <Windows.h>

void* Application::handle = nullptr;

// コンストラクタ
Application::Application()
{
	WNDCLASSEX windowClass = { 0 };
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = WindowProc;
	windowClass.hInstance = hInstance;
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.lpszClassName = L"DXSampleClass";
	RegisterClassEx(&windowClass);

	RECT windowRect = { 0, 0, static_cast<LONG>(pSample->GetWidth()), static_cast<LONG>(pSample->GetHeight()) };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	// Create the window and store a handle to it.
	handle = CreateWindow(
		windowClass.lpszClassName,
		pSample->GetTitle(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		nullptr,        // We have no parent window.
		nullptr,        // We aren't using menus.
		hInstance,
		pSample);
	instance = windowClass.hInstance;
}

// デストラクタ
Application::~Application()
{
	UnregisterClass(name, instance);
}

// ウィンドウのコールバック
long * __stdcall Application::WindowProc(void * hWnd, unsigned int message, unsigned int * wParam, long * lParam)
{
	return 0;
}

