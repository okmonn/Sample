#include "List.h"
#include "../Device/Device.h"
#include "../Release.h"

// コンストラクタ
List::List(std::weak_ptr<Device>dev, const D3D12_COMMAND_LIST_TYPE & type) :
	dev(dev), allo(nullptr), list(nullptr)
{
	CreateAllo(type);
	CreateList(type);
}

// デストラクタ
List::~List()
{
	Release(list);
	Release(allo);
}

// アロケータの生成
long List::CreateAllo(const D3D12_COMMAND_LIST_TYPE & type)
{
	auto hr = dev.lock()->Get()->CreateCommandAllocator(type, IID_PPV_ARGS(&allo));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nアロケータの生成：失敗\n"));
	}

	return  hr;
}

// リストの生成
long List::CreateList(const D3D12_COMMAND_LIST_TYPE & type)
{
	auto hr = dev.lock()->Get()->CreateCommandList(0, type, allo, nullptr, IID_PPV_ARGS(&list));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nリストの生成：失敗\n"));
		return hr;
	}

	list->Close();

	return hr;
}

// リセット
void List::Reset(void)
{
	allo->Reset();
	list->Reset(allo, nullptr);
}
