#include "Compute.h"
#include "../DescriptorMane/DescriptorMane.h"
#include "../Device/Device.h"
#include "../Queue/Queue.h"
#include "../List/List.h"
#include "../Command/Command.h"
#include "../Root/RootCompute.h"
#include "../Pipe/PipeCompute.h"
#include "../Release.h"

// コンストラクタ
Compute::Compute(std::weak_ptr<Device>dev, std::weak_ptr<RootCompute>root, std::weak_ptr<PipeCompute>pipe) :
	descMane(DescriptorMane::Get()), dev(dev), root(root), pipe(pipe), id(0), cId(0), data(nullptr)
{
	com = std::make_unique<Command>(dev, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_COMPUTE);

	CreateRsc();
	CreateView();
	Map();
}

// デストラクタ
Compute::~Compute()
{
}

// ルートシグネチャコンピュートのセット
void Compute::SetRoot(void)
{
	com->GetList()->GetList()->SetComputeRootSignature(root.lock()->Get());
}

// パイプラインコンピュートのセット
void Compute::SetPipe(void)
{
	com->GetList()->GetList()->SetPipelineState(pipe.lock()->Get());
}

// リソースの生成
long Compute::CreateRsc(void)
{
	if (FAILED(descMane.CreateHeap(dev, id, D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE)))
	{
		return S_FALSE;
	}

	D3D12_HEAP_PROPERTIES prop{};
	prop.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_WRITE_COMBINE;
	prop.CreationNodeMask     = 1;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_L0;
	prop.Type                 = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_CUSTOM;
	prop.VisibleNodeMask      = 1;

	D3D12_RESOURCE_DESC desc{};
	desc.Alignment        = 0;
	desc.DepthOrArraySize = 1;
	desc.Dimension        = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Flags            = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
	desc.Format           = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	desc.Height           = 1;
	desc.Layout           = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.MipLevels        = 1;
	desc.SampleDesc       = { 1, 0 };
	desc.Width            = (sizeof(float) + 0xff) &~0xff;

	return descMane.CreateRsc(dev, id, prop, desc, nullptr, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
}

// UAVビューの生成
void Compute::CreateView(void)
{
	D3D12_UNORDERED_ACCESS_VIEW_DESC desc{};
	desc.Buffer.CounterOffsetInBytes = 0;
	desc.Buffer.FirstElement         = 0;
	desc.Buffer.Flags                = D3D12_BUFFER_UAV_FLAGS::D3D12_BUFFER_UAV_FLAG_NONE;
	desc.Buffer.NumElements          = 1;
	desc.Buffer.StructureByteStride  = sizeof(float);
	desc.Format                      = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	desc.ViewDimension               = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_BUFFER;

	dev.lock()->GetDev()->CreateUnorderedAccessView(descMane.GetRsc(id), nullptr, &desc, descMane.GetHeap(id)->GetCPUDescriptorHandleForHeapStart());
}

// マップ
long Compute::Map(void)
{
	auto hr = descMane.GetRsc(id)->Map(0, nullptr, (void**)(&data));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nUAVリソースのマップ：失敗\n"));
	}
	*data = 1.0f;

	return hr;
}

// 実行
void Compute::Do(std::weak_ptr<List>list)
{
	com->GetList()->Reset(nullptr);
	
	SetRoot();
	SetPipe();

	auto heap = descMane.GetHeap(id);

	com->GetList()->GetList()->SetDescriptorHeaps(1, &heap);
	com->GetList()->GetList()->SetComputeRootDescriptorTable(0, heap->GetGPUDescriptorHandleForHeapStart());

	com->GetList()->GetList()->Dispatch(1, 1, 1);

	com->GetList()->Close();

	ID3D12CommandList* n[] = {com->GetList()->GetList()};
	com->GetQueue()->Execute(n, 1);

	auto a = *data;
}
