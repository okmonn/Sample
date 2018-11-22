#include "Application.h"
#include "../Device/Device.h"
#include "../Root/Root.h"
#include "../Pipe/Pipe.h"
#include "../Effector/Effector.h"
#include "../Sound/Sound.h"
#include <d3d12.h>

#pragma comment(lib, "d3d12.lib")

// �R���X�g���N�^
Application::Application()
{
#ifdef _DEBUG
	ID3D12Debug* debug = nullptr;
	D3D12GetDebugInterface(IID_PPV_ARGS(&debug));
	debug->EnableDebugLayer();
#endif

	Create();
}

//�f�X�g���N�^
Application::~Application()
{
}

// �N���X�̐���
void Application::Create(void)
{
	dev    = std::make_shared<Device>();
	root   = std::make_shared<Root>(dev, L"Shader/Delay.hlsl");
	pipe   = std::make_shared<Pipe>(dev, root);
	effect = std::make_shared<Effector>(dev, root, pipe);
	sound  = std::make_shared<Sound>(effect);
}

// �T�E���h�t�@�C���̓ǂݍ���
void Application::Load(const std::string & fileName)
{
	sound->Load(fileName);
}

// �Đ�
void Application::Play(const bool & loop)
{
	sound->Play(loop);
}

// ��~
void Application::Stop(void)
{
	sound->Stop();
}

