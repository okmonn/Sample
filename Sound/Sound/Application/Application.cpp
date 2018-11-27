#include "Application.h"
#include "../Window/Window.h"
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
	win = std::make_shared<Window>();
	dev    = std::make_shared<Device>();
	root   = std::make_shared<Root>(dev, L"Shader/Effect.hlsl");
	pipe   = std::make_shared<Pipe>(dev, root);
	effect = std::make_shared<Effector>(dev, root, pipe);
	sound  = std::make_shared<Sound>(effect);
}

// ���b�Z�[�W�̊m�F
bool Application::CheckMsg(void)
{
	static MSG msg{};

	if (msg.message == WM_QUIT)
	{
		return false;
	}

	//�Ăяo�����X���b�h�����L���Ă���E�B���h�E�ɑ��M���ꂽ���b�Z�[�W�ۗ̕�����Ă��镨���擾
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		//���z�L�[���b�Z�[�W�𕶎����b�Z�[�W�ɕϊ�
		TranslateMessage(&msg);
		//1�̃E�B�h�E�v���V�[�W���Ƀ��b�Z�[�W�𑗏o����
		DispatchMessage(&msg);
	}

	return true;
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

