#include "Window.h"
#include "../Application/Application.h"
#include <Windows.h>
#include <tchar.h>

// �R���X�g���N�^�@
Window::Window(const unsigned int & width, const unsigned int & height) : 
	handle(nullptr), instance(nullptr), name(nullptr)
{
	Create(width, height);
}

// �f�X�g���N�^
Window::~Window()
{
	UnregisterClass(name, (HINSTANCE)(instance));
}

// �^�C�g�����̕ύX
void Window::ChangeTitle(const wchar_t * title)
{
	auto hr = SetWindowText(reinterpret_cast<HWND>(handle), title);
	if (hr == 0)
	{
		OutputDebugString(_T("\n�^�C�g�����̕ύX�F���s\n"));
	}
}

// �E�B���h�E�̐���
void Window::Create(const unsigned int & width, const unsigned int & height)
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
	rect.bottom = static_cast<long>(height);
	rect.left   = 0;
	rect.right  = static_cast<long>(width);
	rect.top    = 0;
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	handle = CreateWindow(wnd.lpszClassName, _T("��������"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		(rect.right - rect.left), (rect.bottom - rect.top), nullptr, nullptr, wnd.hInstance, nullptr);
	if (handle == nullptr)
	{
		return;
	}

	instance = wnd.hInstance;
	name     = wnd.lpszClassName;
	
	ShowWindow(reinterpret_cast<HWND>(handle), SW_SHOW);
}
