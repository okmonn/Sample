#include "Device.h"
#include "../Release.h"

const D3D_FEATURE_LEVEL levels[]=
{
	D3D_FEATURE_LEVEL_12_1,
	D3D_FEATURE_LEVEL_12_0,
	D3D_FEATURE_LEVEL_11_1,
	D3D_FEATURE_LEVEL_11_0
};

// コンストラクタ
Device::Device() : 
	dev(nullptr)
{
	CreateDev();
}

// デストラクタ
Device::~Device()
{
	Release(dev);
}

// デバイスの生成
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
