#include "Application.h"
#include "../Window/Window.h"
#include "../Input/Input.h"
#include "../Audio/SoundMane.h"
#include "Union/Union.h"
#include <d3d12.h>

#pragma comment(lib, "d3d12.lib")

void* Application::winHandle     = nullptr;
unsigned int Application::width  = 0;
unsigned int Application::height = 0;

int p = 0;

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
	sound = std::make_unique<SoundMane>();
	un    = std::make_unique<Union>();

	sound->Load("妖はるかなり～妖々夢.wav", p);
	sound->Play(p);
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
	
	//呼び出し側スレッドが所有しているウィンドウに送信されたメッセージの保留されている物を取得
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		//仮想キーメッセージを文字メッセージに変換
		TranslateMessage(&msg);
		//1つのウィドウプロシージャにメッセージを送出する
		DispatchMessage(&msg);
	}

	return true;
}

// キーの入力
bool Application::CheckKey(const int & i)
{
	return input->CheckKey(i);
}

// 描画・処理
void Application::Render(void)
{
	un->Set();
	un->Do();
}
