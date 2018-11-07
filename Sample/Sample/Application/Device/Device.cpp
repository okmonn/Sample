#include "Device.h"
#include "../Release.h"
#include <dxgi1_6.h>

#pragma comment(lib, "dxgi.lib")

// 機能レベル一覧
D3D_FEATURE_LEVEL levels[] =
{
	D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_12_1,
	D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_12_0,
	D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_1,
	D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0,
};

// コンストラクタ
Device::Device() : 
	dev(nullptr), adapter(nullptr)
{
	Init();
}

// デストラクタ
Device::~Device()
{
	Release(dev);
	Release(adapter);
	Release(factory);
}

// ファクトリーの生成
long Device::CreateFactory(void)
{
	auto hr = CreateDXGIFactory(IID_PPV_ARGS(&factory));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nファクトリーの生成：失敗\n"));
	}

	return hr;
}

// デバイスの生成
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
				OutputDebugString(_T("\nデバイスの生成：成功\n"));
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

// 初期化
void Device::Init(void)
{
	CreateFactory();
	CreateDev();
}
