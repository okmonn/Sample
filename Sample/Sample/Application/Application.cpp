#include "Application.h"
#include "../Window/Window.h"
#include "../Input/Input.h"
#include "Union/Union.h"
#include <d3d12.h>
#include <dxgi1_6.h>

#pragma comment(lib, "d3d12.lib")

void* Application::winHandle     = nullptr;
unsigned int Application::width  = 0;
unsigned int Application::height = 0;
std::unique_ptr<Union> Application::un = nullptr;

// コンストラクタ
Application::Application(const unsigned int & width, const unsigned int & height)
{
#ifdef _DEBUG
	ID3D12Debug* debug = nullptr;
	D3D12GetDebugInterface(IID_PPV_ARGS(&debug));
	debug->EnableDebugLayer();
#endif
	this->width  = width;
	this->height = height;

	win   = std::make_unique<Window>(&winHandle);
	input = std::make_unique<Input>();
	un    = std::make_unique<Union>();
}

// デストラクタ
Application::~Application()
{
}

// メッセージの確認
bool Application::CheckMsg(void)
{
	static MSG msg{};

	if (msg.message == WM_QUIT)
	{
		return false;
	}
	else
	{
		//呼び出し側スレッドが所有しているウィンドウに送信されたメッセージの保留されている物を取得
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			//仮想キーメッセージを文字メッセージに変換
			TranslateMessage(&msg);
			//1つのウィドウプロシージャにメッセージを送出する
			DispatchMessage(&msg);
		}
	}

	return true;
}

// キーの入力
bool Application::CheckKey(const int & i)
{
	return input->CheckKey(i);
}

// ウィンドウのコールバック
long Application::WindowProc(void * hWnd, unsigned int message, long wParam, long lParam)
{
	switch (message)
	{
	case WM_CREATE:
		break;

	case WM_PAINT:
		// 処理
		if (un != nullptr)
		{
			un->Set();
			un->Do();
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	default:
		break;
	}

	return DefWindowProc((HWND)hWnd, message, wParam, lParam);
}


