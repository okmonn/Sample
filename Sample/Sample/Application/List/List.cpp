#include "List.h"
#include "../Application.h"
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
	list->Close();
	return hr;
}

// 初期化
void List::Init(const D3D12_COMMAND_LIST_TYPE & type)
{
	CreateAllo(type);
	CreateList(type);
}

// リセット
void List::Reset(ID3D12PipelineState * pipe)
{
	allo->Reset();
	list->Reset(allo, pipe);
}

// ビューポートのセット
void List::SetViewport(void)
{
	D3D12_VIEWPORT view{};
	view.Height   = static_cast<float>(Application::GetHeight());
	view.MaxDepth = 1.0f;
	view.MinDepth = 0.0f;
	view.TopLeftX = 0.0f;
	view.TopLeftY = 0.0f;
	view.Width    = static_cast<float>(Application::GetWidth());

	list->RSSetViewports(1, &view);
}

// シザーのセット
void List::SetScissor(void)
{
	RECT scissor{};
	scissor.bottom = static_cast<LONG>(Application::GetHeight());
	scissor.left   = 0;
	scissor.right  = static_cast<LONG>(Application::GetWidth());
	scissor.top    = 0;

	list->RSSetScissorRects(1, &scissor);
}

// バリアのセット
void List::SetBarrier(const D3D12_RESOURCE_STATES & befor, const D3D12_RESOURCE_STATES & affter, ID3D12Resource * rsc)
{
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type                   = D3D12_RESOURCE_BARRIER_TYPE::D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags                  = D3D12_RESOURCE_BARRIER_FLAGS::D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource   = rsc;
	barrier.Transition.StateBefore = befor;
	barrier.Transition.StateAfter  = affter;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_FLAGS::D3D12_RESOURCE_BARRIER_FLAG_NONE;

	list->ResourceBarrier(1, &barrier);
}
