#include <xaudio2.h>
#include "XAudio2.h"
#include "../Application/Release.h"

#pragma comment(lib, "xaudio2.lib")

// ����}�N��
#define Destroy(X) { if((X) != nullptr) (X)->DestroyVoice(); (X) = nullptr; }

// �R���X�g���N�^
XAudio2::XAudio2() : 
	audio(nullptr), mastering(nullptr)
{
	Init();
}

// �f�X�g���N�^
XAudio2::~XAudio2()
{
	Destroy(mastering);
	Release(audio);

	CoUninitialize();
}

// COM�̏�����
long XAudio2::InitCom(void)
{
	auto hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nCOM�̏������F���s\n"));
	}

	return hr;
}

// �I�[�f�B�I�̐���
long XAudio2::CreateAudio(void)
{
	auto hr = XAudio2Create(&audio);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nXAudio2�̐����F���s\n"));
	}

	return hr;
}

// �}�X�^�����O�̐���
long XAudio2::CreateMastering(void)
{
	auto hr = audio->CreateMasteringVoice(&mastering);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n�}�X�^�����O�{�C�X�̐����F���s\n"));
	}

	return hr;
}

// ������
void XAudio2::Init(void)
{
	InitCom();
	CreateAudio();
	CreateMastering();
}
