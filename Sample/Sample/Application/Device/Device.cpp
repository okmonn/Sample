#include "Device.h"
#include "../Release.h"
#include <dxgi1_6.h>

#pragma comment(lib, "dxgi.lib")

// �@�\���x���ꗗ
D3D_FEATURE_LEVEL levels[] =
{
	D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_12_1,
	D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_12_0,
	D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_1,
	D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0,
};

// �R���X�g���N�^
Device::Device() : 
	dev(nullptr), adapter(nullptr)
{
	Init();
}

// �f�X�g���N�^
Device::~Device()
{
	Release(dev);
	Release(adapter);
	Release(factory);
}

// �t�@�N�g���[�̐���
long Device::CreateFactory(void)
{
	auto hr = CreateDXGIFactory(IID_PPV_ARGS(&factory));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n�t�@�N�g���[�̐����F���s\n"));
	}

	return hr;
}

// �f�o�C�X�̐���
long Device::CreateDev(void)
{
	auto hr = S_OK;
	for (unsigned int adapterIndex = 0; factory->EnumAdapters1(adapterIndex, &adapter) != DXGI_ERROR_NOT_FOUND; ++adapterIndex)
	{
		for (auto& i : levels)
		{
			hr = D3D12CreateDevice(adapter, i, IID_PPV_ARGS(&dev));
			if (hr == S_OK)
			{
				OutputDebugString(_T("\n�f�o�C�X�̐����F����\n"));
				break;
			}
		}
		if (hr == S_OK)
		{
			break;
		}
	}

	return hr;
}

// ������
void Device::Init(void)
{
	CreateFactory();
	CreateDev();
}
