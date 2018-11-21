#include "List.h"
#include "../Device/Device.h"
#include "../Release.h"

// �R���X�g���N�^
List::List(std::weak_ptr<Device>dev, const D3D12_COMMAND_LIST_TYPE & type) :
	dev(dev), allo(nullptr), list(nullptr)
{
	CreateAllo(type);
	CreateList(type);
}

// �f�X�g���N�^
List::~List()
{
	Release(list);
	Release(allo);
}

// �A���P�[�^�̐���
long List::CreateAllo(const D3D12_COMMAND_LIST_TYPE & type)
{
	auto hr = dev.lock()->Get()->CreateCommandAllocator(type, IID_PPV_ARGS(&allo));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n�A���P�[�^�̐����F���s\n"));
	}

	return  hr;
}

// ���X�g�̐���
long List::CreateList(const D3D12_COMMAND_LIST_TYPE & type)
{
	auto hr = dev.lock()->Get()->CreateCommandList(0, type, allo, nullptr, IID_PPV_ARGS(&list));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n���X�g�̐����F���s\n"));
		return hr;
	}

	list->Close();

	return hr;
}

// ���Z�b�g
void List::Reset(void)
{
	allo->Reset();
	list->Reset(allo, nullptr);
}
