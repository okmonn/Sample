#include "Effector.h"
#include "../Device/Device.h"
#include "../Queue/Queue.h"
#include "../List/List.h"
#include "../Fence/Fence.h"
#include "../Root/Root.h"
#include "../Pipe/Pipe.h"
#include "../Release.h"

// リソース数
#define RSC_MAX 3

// データサイズ
#define DATA_MAX 1764

// コンストラクタ
Effector::Effector(std::weak_ptr<Device>dev, std::weak_ptr<Root>root, std::weak_ptr<Pipe>pipe) :
	dev(dev), root(root), pipe(pipe), heap(nullptr), index(0)
{
	param = {};
	info.clear();

	queue = std::make_shared<Queue>(dev, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_COMPUTE);
	list  = std::make_unique<List>(dev, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_COMPUTE);
	fence = std::make_unique<Fence>(dev, queue);

	Init();
}

// デストラクタ
Effector::~Effector()
{
	for (auto itr = info.begin(); itr != info.end(); ++itr)
	{
		Release(itr->second.rsc);
	}

	Release(heap);
}

// ヒープの生成
long Effector::CreateHeap(void)
{
	D3D12_DESCRIPTOR_HEAP_DESC desc{};
	desc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.NodeMask       = 0;
	desc.NumDescriptors = RSC_MAX;
	desc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	auto hr = dev.lock()->Get()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&heap));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nエフェクト用ヒープの生成：失敗\n"));
	}

	return hr;
}

// リソースの生成
long Effector::CreateCbvRsc(const std::string & name, const unsigned int & size)
{
	D3D12_HEAP_PROPERTIES prop{};
	prop.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.CreationNodeMask     = 1;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	prop.Type                 = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD;
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
	desc.Width            = (size + 0xff) &~0xff;

	auto hr = dev.lock()->Get()->CreateCommittedResource(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
		&desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&info[name].rsc));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nエフェクト用CBVリソースの生成：失敗\n"));
		return hr;
	}

	info[name].index = index++;

	return hr;
}

// UAVリソースの生成
long Effector::CreateUavRsc(const std::string & name, const unsigned int & size)
{
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
	desc.Width            = (size + 0xff) &~0xff;

	auto hr = dev.lock()->Get()->CreateCommittedResource(&prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
		&desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS, nullptr, IID_PPV_ARGS(&info[name].rsc));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nエフェクト用UAVリソースの生成：失敗\n"));
		return hr;
	}

	info[name].index = index++;

	return hr;
}

// CBVの生成
void Effector::CreateConstantView(const std::string & name, const unsigned int & size)
{
	D3D12_CONSTANT_BUFFER_VIEW_DESC desc{};
	desc.BufferLocation = info[name].rsc->GetGPUVirtualAddress();
	desc.SizeInBytes    = (size + 0xff) &~0xff;

	auto handle = heap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += info[name].index * dev.lock()->Get()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	dev.lock()->Get()->CreateConstantBufferView(&desc, handle);
}

// UAVの生成
void Effector::CreateUnorderView(const std::string & name, const unsigned int & stride, const unsigned int & num)
{
	D3D12_UNORDERED_ACCESS_VIEW_DESC desc{};
	desc.ViewDimension              = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_BUFFER;
	desc.Format                     = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	desc.Buffer.NumElements         = num;
	desc.Buffer.StructureByteStride = stride;

	auto handle = heap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += info[name].index * dev.lock()->Get()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	
	dev.lock()->Get()->CreateUnorderedAccessView(info[name].rsc, nullptr, &desc, handle);
}

// マップ
long Effector::Map(const std::string & name)
{
	D3D12_RANGE range{ 0, 1 };

	auto hr = info[name].rsc->Map(0, &range, reinterpret_cast<void**>(&info[name].data));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nエフェクト用リソースのマップ：失敗\n"));
	}

	return hr;
}

// CBVの生成
void Effector::CBV(const std::string & name, const unsigned int & size)
{
	CreateCbvRsc(name, size);
	CreateConstantView(name, size);
	Map(name);
}

// UAVの生成
void Effector::UAV(const std::string & name, const unsigned int & stride, const unsigned int & num)
{
	CreateUavRsc(name, stride * num);
	CreateUnorderView(name, stride, num);
	Map(name);
}

// 初期化
void Effector::Init(void)
{
	CreateHeap();

	CBV("b0", sizeof(Param));
	UAV("u0", sizeof(float), DATA_MAX);
	UAV("u1", sizeof(float), DATA_MAX);
}

// 実行
void Effector::Execution(const std::vector<float> & wave, std::vector<float> & adaptation, const unsigned int & index, const unsigned int & sample)
{
	param.attenuation = 0.5f;
	param.time = 0.375f;
	param.loop = 10;
	param.index = index;
	param.sample = sample;

	memcpy(info["b0"].data, &param, sizeof(Param));
	memcpy(info["u0"].data, &wave[0], sizeof(float) * wave.size());
	memset(info["u1"].data, 0, sizeof(float) * wave.size());

	list->Reset();

	list->GetList()->SetComputeRootSignature(root.lock()->Get());
	list->GetList()->SetPipelineState(pipe.lock()->Get());

	list->GetList()->SetDescriptorHeaps(1, &heap);

	auto size = dev.lock()->Get()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	auto handle = heap->GetGPUDescriptorHandleForHeapStart();
	handle.ptr = heap->GetGPUDescriptorHandleForHeapStart().ptr + size * info["b0"].index;
	list->GetList()->SetComputeRootDescriptorTable(0, handle);
	handle.ptr = heap->GetGPUDescriptorHandleForHeapStart().ptr + size * info["u0"].index;
	list->GetList()->SetComputeRootDescriptorTable(1, handle);
	handle.ptr = heap->GetGPUDescriptorHandleForHeapStart().ptr + size * info["u1"].index;
	list->GetList()->SetComputeRootDescriptorTable(2, handle);

	list->GetList()->Dispatch(static_cast<unsigned int>(wave.size()), 1, 1);

	list->GetList()->Close();

	ID3D12CommandList* com[] = {
		list->GetList(),
	};

	queue->Get()->ExecuteCommandLists(1, com);

	fence->Wait();

	adaptation.assign(info["u1"].data, info["u1"].data + wave.size());
}
