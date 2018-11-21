#include "Device.h"
#include "../Release.h"

#pragma comment(lib, "d3d12.lib")

const D3D_FEATURE_LEVEL levels[]=
{
	D3D_FEATURE_LEVEL_12_1,
	D3D_FEATURE_LEVEL_12_0,
	D3D_FEATURE_LEVEL_11_1,
	D3D_FEATURE_LEVEL_11_0
};

// �R���X�g���N�^
Device::Device() : 
	dev(nullptr)
{
	CreateDev();
}

// �f�X�g���N�^
Device::~Device()
{
	Release(dev);
}

// �f�o�C�X�̐���
long Device::CreateDev(void)
{
	auto hr = S_OK;

	for (auto& i : levels)
	{
		hr = D3D12CreateDevice(nullptr, i, IID_PPV_ARGS(&dev));
		if (!FAILED(hr))
		{
			break;
		}
	}

	return hr;
}