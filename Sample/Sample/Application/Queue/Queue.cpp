#include "Queue.h"
#include "../Device/Device.h"
#include "../Release.h"

// �R���X�g���N�^
Queue::Queue(std::weak_ptr<Device>dev, const D3D12_COMMAND_LIST_TYPE & type) :
	dev(dev), queue(nullptr)
{
	Create(type);
}

// �f�X�g���N�^
Queue::~Queue()
{
	Release(queue);
}

// �R�}���h�L���[�̐���
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
		OutputDebugString(_T("\n�R�}���h�L���[�̐����F���s\n"));
	}

	return hr;
}

// �R�}���h���X�g�̎��s
void Queue::Execute(ID3D12CommandList ** list, const unsigned int & num)
{
	queue->ExecuteCommandLists(num, list);
}
