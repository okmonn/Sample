#include "Application.h"
#include "../Window/Window.h"
#include "../Device/Device.h"
#include "../Root/Root.h"
#include "../Pipe/Pipe.h"
#include "../Effector/Effector.h"
#include "../Sound/Sound.h"
#include <d3d12.h>

#pragma comment(lib, "d3d12.lib")

// コンストラクタ
Application::Application()
{
#ifdef _DEBUG
	ID3D12Debug* debug = nullptr;
	D3D12GetDebugInterface(IID_PPV_ARGS(&debug));
	debug->EnableDebugLayer();
#endif

	Create();
}

//デストラクタ
Application::~Application()
{
}

// クラスの生成
void Application::Create(void)
{
	win = std::make_shared<Window>();
	dev    = std::make_shared<Device>();
	root   = std::make_shared<Root>(dev, L"Shader/Effect.hlsl");
	pipe   = std::make_shared<Pipe>(dev, root);
	effect = std::make_shared<Effector>(dev, root, pipe);
	sound  = std::make_shared<Sound>(effect);
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

// サウンドファイルの読み込み
void Application::Load(const std::string & fileName)
{
	sound->Load(fileName);
}

// 再生
void Application::Play(const bool & loop)
{
	sound->Play(loop);
}

// 停止
void Application::Stop(void)
{
	sound->Stop();
}

