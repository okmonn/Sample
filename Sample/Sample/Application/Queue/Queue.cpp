#include "Queue.h"
#include "../Device/Device.h"
#include "../Release.h"

// コンストラクタ
Queue::Queue(std::weak_ptr<Device>dev, const D3D12_COMMAND_LIST_TYPE & type) :
	dev(dev), queue(nullptr)
{
	Create(type);
}

// デストラクタ
Queue::~Queue()
{
	Release(queue);
}

// コマンドキューの生成
long Queue::Create(const D3D12_COMMAND_LIST_TYPE & type)
{
	D3D12_COMMAND_QUEUE_DESC desc{};
	desc.Flags    = D3D12_COMMAND_QUEUE_FLAGS::D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.NodeMask = 0;
	desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY::D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	desc.Type     = type;

	auto hr = dev.lock()->GetDev()->CreateCommandQueue(&desc, IID_PPV_ARGS(&queue));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nコマンドキューの生成：失敗\n"));
	}

	return hr;
}

// コマンドリストの実行
void Queue::Execute(ID3D12CommandList ** list, const unsigned int & num)
{
	queue->ExecuteCommandLists(num, list);
}
