#include "List.h"
#include "../Device/Device.h"
#include "../Release.h"

// コンストラクタ
List::List(std::weak_ptr<Device>dev, const D3D12_COMMAND_LIST_TYPE & type) :
	dev(dev), allo(nullptr), list(nullptr)
{
	Init(type);
}

// デストラクタ
List::~List()
{
	Release(list);
	Release(allo);
}

// コマンドアロケータの生成
long List::CreateAllo(const D3D12_COMMAND_LIST_TYPE & type)
{
	auto hr = dev.lock()->GetDev()->CreateCommandAllocator(type, IID_PPV_ARGS(&allo));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nコマンドアロケータの生成：失敗\n"));
	}

	return hr;
}

// コマンドリストの生成
long List::CreateList(const D3D12_COMMAND_LIST_TYPE & type)
{
	auto hr = dev.lock()->GetDev()->CreateCommandList(0, type, allo, nullptr, IID_PPV_ARGS(&list));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nコマンドリストの生成：失敗\n"));
	}

	return hr;
}

// 初期化
void List::Init(const D3D12_COMMAND_LIST_TYPE & type)
{
	CreateAllo(type);
	CreateList(type);
}
