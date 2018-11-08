#include "Render.h"
#include "../Device/Device.h"
#include "../List/List.h"
#include "../Swap/Swap.h"
#include "../Release.h"
#include <dxgi1_6.h>

// �R���X�g���N�^
Render::Render(std::weak_ptr<Device>dev, std::weak_ptr<Swap>swap) :
	dev(dev), swap(swap), heap(nullptr)
{
	Init();
}

// �f�X�g���N�^
Render::~Render()
{
	for (auto& i : rsc)
	{
		Release(i);
	}
	Release(heap);
}

// �q�[�v�̐���
long Render::CreateHeap(void)
{
	DXGI_SWAP_CHAIN_DESC1 tmp{};
	swap.lock()->Get()->GetDesc1(&tmp);

	D3D12_DESCRIPTOR_HEAP_DESC desc{};
	desc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	desc.NodeMask       = 0;
	desc.NumDescriptors = tmp.BufferCount;
	desc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

	auto hr = dev.lock()->GetDev()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&heap));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n�����_�[�p�q�[�v�̐����F���s\n"));
	}

	return hr;
}

// ���\�[�X�ƃ����_�[�^�[�Q�b�g�r���[�̐���
long Render::CreateRscView(void)
{
	DXGI_SWAP_CHAIN_DESC1 tmp{};
	swap.lock()->Get()->GetDesc1(&tmp);

	rsc.resize(tmp.BufferCount);

	D3D12_RENDER_TARGET_VIEW_DESC desc{};
	desc.Format               = tmp.Format;
	desc.ViewDimension        = D3D12_RTV_DIMENSION::D3D12_RTV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipSlice   = 0;
	desc.Texture2D.PlaneSlice = 0;

	D3D12_CPU_DESCRIPTOR_HANDLE handle = heap->GetCPUDescriptorHandleForHeapStart();
	for (unsigned int i = 0; i < rsc.size(); ++i)
	{
		auto hr = swap.lock()->Get()->GetBuffer(i, IID_PPV_ARGS(&rsc[i]));
		if (FAILED(hr))
		{
			OutputDebugString(_T("\n�����_�[�p���\�[�X�̐����F���s\n"));
			break;
		}

		//�����_�[�^�[�Q�b�g�̐���
		dev.lock()->GetDev()->CreateRenderTargetView(rsc[i], &desc, handle);

		//�n���h���ʒu�̍X�V
		handle.ptr += dev.lock()->GetDev()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}

	return S_OK;
}

// ������
void Render::Init(void)
{
	CreateHeap();
	CreateRscView();
}

// �����_�[�^�[�Q�b�g�̃Z�b�g
void Render::SetRender(ID3D12GraphicsCommandList * list, ID3D12DescriptorHeap * depth, const float & color)
{
	//�q�[�v�̐擪�n���h���̎擾
	D3D12_CPU_DESCRIPTOR_HANDLE handle = heap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += dev.lock()->GetDev()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV) * swap.lock()->Get()->GetCurrentBackBufferIndex();

	//�����_�[�^�[�Q�b�g�̃Z�b�g
	list->OMSetRenderTargets(1, &handle, false, &depth->GetCPUDescriptorHandleForHeapStart());

	//�����_�[�^�[�Q�b�g�̃N���A
	list->ClearRenderTargetView(handle, &color, 0, nullptr);
}
