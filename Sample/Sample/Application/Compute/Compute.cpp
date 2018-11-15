#include "Compute.h"
#include "../DescriptorMane/DescriptorMane.h"
#include "../Device/Device.h"
#include "../Queue/Queue.h"
#include "../List/List.h"
#include "../Command/Command.h"
#include "../Fence/Fence.h"
#include "../Root/RootCompute.h"
#include "../Pipe/PipeCompute.h"
#include "../Release.h"
#include <vector>

// コンストラクタ
Compute::Compute(std::weak_ptr<Device>dev, std::weak_ptr<RootCompute>root, std::weak_ptr<PipeCompute>pipe) :
	descMane(DescriptorMane::Get()), dev(dev), root(root), pipe(pipe), uav(0), copy(0), data(nullptr)
{
	com   = std::make_unique<Command>(dev, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_COMPUTE);
	fence = std::make_unique<Fence>(dev, com->GetQueue());

	Init();
}

// デストラクタ
Compute::~Compute()
{

}

// UAV用リソースの生成
long Compute::CreateUavRsc(void)
{
	if (FAILED(descMane.CreateHeap(dev, uav, D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE)))
	{
		return S_FALSE;
	}

	D3D12_HEAP_PROPERTIES prop{};
	prop.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
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

	return descMane.CreateRsc(dev, uav, prop, desc, nullptr, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
}

// UAVビューの生成
void Compute::CreateView(void)
{
	D3D12_UNORDERED_ACCESS_VIEW_DESC desc{};
	desc.ViewDimension              = D3D12_UAV_DIMENSION_BUFFER;
	desc.Format                     = DXGI_FORMAT_UNKNOWN;
	desc.Buffer.NumElements         = 14;
	desc.Buffer.StructureByteStride = sizeof(float);

	dev.lock()->GetDev()->CreateUnorderedAccessView(descMane.GetRsc(uav), nullptr, &desc, descMane.GetHeap(uav)->GetCPUDescriptorHandleForHeapStart());
}

// コピーリソースの生成
long Compute::CreateCopyRsc(void)
{
	D3D12_HEAP_PROPERTIES prop{};
	prop.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.CreationNodeMask     = 1;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	prop.Type                 = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_READBACK;
	prop.VisibleNodeMask      = 1;

	D3D12_RESOURCE_DESC desc{};
	desc.Alignment        = 0;
	desc.DepthOrArraySize = 1;
	desc.Dimension        = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Flags            = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;
	desc.Format           = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	desc.Height           = 1;
	desc.Layout           = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.MipLevels        = 1;
	desc.SampleDesc       = { 1, 0 };
	desc.Width            = (sizeof(float) + 0xff) &~0xff;

	return descMane.CreateRsc(dev, copy, prop, desc, nullptr, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST);
}

// 初期化
void Compute::Init(void)
{
	CreateUavRsc();
	CreateCopyRsc();
	CreateView();
}

// マップ
long Compute::Map(void)
{
	D3D12_RANGE range{ 0, 1 };
	long hr = descMane.GetRsc(copy)->Map(0, &range, reinterpret_cast<void**>(&data));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nコンピュート用リソースのマップ：失敗\n"));
		return hr;
	}

	descMane.GetRsc(copy)->Unmap(0, nullptr);

	return hr;
}

// 実行
void Compute::Execution(void)
{
	D3D12_RANGE range{0, 1};
	descMane.GetRsc(uav)->Map(0, &range, (void**)&data);
	const float a[] = {
		100,100,100,100,100,100,100,100,100,100,100,100,100,100,
	};
	memcpy(data, &a[0], sizeof(a));
	descMane.GetRsc(uav)->Unmap(0, &range);

	com->GetList()->Reset(nullptr);

	com->GetList()->GetList()->SetComputeRootSignature(root.lock()->Get());
	com->GetList()->SetPipe(pipe.lock()->Get());

	auto heap = descMane.GetHeap(uav);
	com->GetList()->GetList()->SetDescriptorHeaps(1, &heap);
	com->GetList()->GetList()->SetComputeRootDescriptorTable(0, heap->GetGPUDescriptorHandleForHeapStart());

	com->GetList()->GetList()->Dispatch(1, 1, 1);

	com->GetList()->SetBarrier(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_SOURCE, descMane.GetRsc(uav));
	
	com->GetList()->GetList()->CopyResource(descMane.GetRsc(copy), descMane.GetRsc(uav));

	com->GetList()->Close();
	ID3D12CommandList* list = com->GetList()->GetList();
	com->GetQueue()->Execute(&list, 1);

	fence->Wait();

	Map();
	float* ptr = &data[0];
	std::vector<float> p(ptr, ptr + 14);
	int n = 0;

	{
		D3D12_RANGE range{ 0, 1 };
		descMane.GetRsc(uav)->Map(0, &range, (void**)&data);
		memset(data, 0, sizeof(float) * 14);
		descMane.GetRsc(uav)->Unmap(0, &range);

		com->GetList()->Reset(nullptr);

		com->GetList()->GetList()->SetComputeRootSignature(root.lock()->Get());
		com->GetList()->SetPipe(pipe.lock()->Get());

		auto heap = descMane.GetHeap(uav);
		com->GetList()->GetList()->SetDescriptorHeaps(1, &heap);
		com->GetList()->GetList()->SetComputeRootDescriptorTable(0, heap->GetGPUDescriptorHandleForHeapStart());

		com->GetList()->GetList()->Dispatch(1, 1, 1);

		com->GetList()->SetBarrier(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
			D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_SOURCE, descMane.GetRsc(uav));

		com->GetList()->GetList()->CopyResource(descMane.GetRsc(copy), descMane.GetRsc(uav));

		com->GetList()->Close();
		ID3D12CommandList* list = com->GetList()->GetList();
		com->GetQueue()->Execute(&list, 1);

		fence->Wait();

		Map();
		float* ptr = &data[0];
		std::vector<float> p(ptr, ptr + 14);

		int n = 0;
	}
}
