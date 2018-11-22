#include "Application.h"
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
	dev    = std::make_shared<Device>();
	root   = std::make_shared<Root>(dev, L"Shader/Delay.hlsl");
	pipe   = std::make_shared<Pipe>(dev, root);
	effect = std::make_shared<Effector>(dev, root, pipe);
	sound  = std::make_shared<Sound>(effect);
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

