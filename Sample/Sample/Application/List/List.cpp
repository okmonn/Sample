#include "List.h"
#include "../Device/Device.h"
#include "../Release.h"

// �R���X�g���N�^
List::List(std::weak_ptr<Device>dev, const D3D12_COMMAND_LIST_TYPE & type) :
	dev(dev), allo(nullptr), list(nullptr)
{
	Init(type);
}

// �f�X�g���N�^
List::~List()
{
	Release(list);
	Release(allo);
}

// �R�}���h�A���P�[�^�̐���
long List::CreateAllo(const D3D12_COMMAND_LIST_TYPE & type)
{
	auto hr = dev.lock()->GetDev()->CreateCommandAllocator(type, IID_PPV_ARGS(&allo));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n�R�}���h�A���P�[�^�̐����F���s\n"));
	}

	return hr;
}

// �R�}���h���X�g�̐���
long List::CreateList(const D3D12_COMMAND_LIST_TYPE & type)
{
	auto hr = dev.lock()->GetDev()->CreateCommandList(0, type, allo, nullptr, IID_PPV_ARGS(&list));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n�R�}���h���X�g�̐����F���s\n"));
	}

	return hr;
}

// ������
void List::Init(const D3D12_COMMAND_LIST_TYPE & type)
{
	CreateAllo(type);
	CreateList(type);
}
