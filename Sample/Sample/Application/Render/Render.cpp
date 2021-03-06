#include "Render.h"
#include "../Device/Device.h"
#include "../List/List.h"
#include "../Swap/Swap.h"
#include "../Release.h"
#include <dxgi1_6.h>

// コンストラクタ
Render::Render(std::weak_ptr<Device>dev, std::weak_ptr<Swap>swap) :
	dev(dev), swap(swap), heap(nullptr)
{
	Init();
}

// デストラクタ
Render::~Render()
{
	for (auto& i : rsc)
	{
		Release(i);
	}
	Release(heap);
}

// ヒープの生成
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
		OutputDebugString(_T("\nレンダー用ヒープの生成：失敗\n"));
	}

	return hr;
}

// リソースとレンダーターゲットビューの生成
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
			OutputDebugString(_T("\nレンダー用リソースの生成：失敗\n"));
			break;
		}

		//レンダーターゲットの生成
		dev.lock()->GetDev()->CreateRenderTargetView(rsc[i], &desc, handle);

		//ハンドル位置の更新
		handle.ptr += dev.lock()->GetDev()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}

	return S_OK;
}

// 初期化
void Render::Init(void)
{
	CreateHeap();
	CreateRscView();
}

// レンダーターゲットのセット
void Render::SetRender(ID3D12GraphicsCommandList * list, ID3D12DescriptorHeap * depth, const float & color)
{
	//ヒープの先頭ハンドルの取得
	D3D12_CPU_DESCRIPTOR_HANDLE handle = heap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += dev.lock()->GetDev()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV) * swap.lock()->Get()->GetCurrentBackBufferIndex();

	//レンダーターゲットのセット
	list->OMSetRenderTargets(1, &handle, false, &depth->GetCPUDescriptorHandleForHeapStart());

	//レンダーターゲットのクリア
	list->ClearRenderTargetView(handle, &color, 0, nullptr);
}
