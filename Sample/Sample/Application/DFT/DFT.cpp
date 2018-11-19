#include "DFT.h"
#include "../../Audio/SoundLoader.h"
#include "../../Audio/XAudio2.h"
#include "../../Audio/VoiceCallback.h"
#include "../../Audio/Sound.h"
#include "../DescriptorMane/DescriptorMane.h"
#include "../Device/Device.h"
#include "../Queue/Queue.h"
#include "../List/List.h"
#include "../Command/Command.h"
#include "../Fence/Fence.h"
#include "../Root/RootCompute.h"
#include "../Pipe/PipeCompute.h"
#include "../Release.h"

// ���\�[�X��
#define RSC_CNT 2

// �f�[�^��
#define DATA_MAX 2940

// �R���X�g���N�^
DFT::DFT(std::weak_ptr<Device>dev, std::weak_ptr<RootCompute>root, std::weak_ptr<PipeCompute>pipe) : 
	loader(SoundLoader::Get()), descMane(DescriptorMane::Get()), dev(dev), root(root), pipe(pipe), 
	heapID(0), index(0), waveIndex(0)
{
	com = std::make_unique<Command>(this->dev, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_COMPUTE);
	fence = std::make_unique<Fence>(dev, com->GetQueue());

	rsc.clear();

	Init();
}

// �f�X�g���N�^
DFT::~DFT()
{
}

// �g�`�ǂݍ���
void DFT::Load(const std::string & fileName)
{
	audio = std::make_unique<XAudio2>();
	sound = std::make_unique<Sound>(audio);
	sound->Load(fileName);
	sound->Play(false);
}

// �q�[�v�̐���
long DFT::CreateHeap(void)
{
	return descMane.CreateHeap(dev, heapID, D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, RSC_CNT);
}

// CBV���\�[�X�̐���
long DFT::CreateCbvRsc(const std::string& name)
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
	desc.Width            = (sizeof(float) * DATA_MAX + 0xff) &~0xff;

	rsc[name].id = index++;

	return descMane.CreateRsc(dev, rsc[name].id, prop, desc);
}

// CBV�r���[�̐���
void DFT::CreateCbvView(const std::string& name)
{
	D3D12_CONSTANT_BUFFER_VIEW_DESC desc{};
	desc.BufferLocation = descMane.GetRsc(rsc[name].id)->GetGPUVirtualAddress();
	desc.SizeInBytes = (sizeof(float) * DATA_MAX + 0xff) &~0xff;

	auto handle = descMane.GetHeap(heapID)->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += dev.lock()->GetDev()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * rsc[name].id;

	dev.lock()->GetDev()->CreateConstantBufferView(&desc, handle);
}

// UAV���\�[�X�̐���
long DFT::CreateUavRsc(const std::string& name)
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
	desc.Width            = (sizeof(float) * DATA_MAX + 0xff) &~0xff;

	rsc[name].id = index++;

	return descMane.CreateRsc(dev, rsc[name].id, prop, desc, nullptr, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
}

// UAV�r���[�̐���
void DFT::CreateUavView(const std::string& name)
{
	D3D12_UNORDERED_ACCESS_VIEW_DESC desc{};
	desc.ViewDimension              = D3D12_UAV_DIMENSION_BUFFER;
	desc.Format                     = DXGI_FORMAT_UNKNOWN;
	desc.Buffer.NumElements         = DATA_MAX;
	desc.Buffer.StructureByteStride = sizeof(float);

	auto handle = descMane.GetHeap(heapID)->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += dev.lock()->GetDev()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * rsc[name].id;

	dev.lock()->GetDev()->CreateUnorderedAccessView(descMane.GetRsc(rsc[name].id), nullptr, &desc, handle);
}

// ���\�[�X�̃}�b�v
long DFT::Map(const std::string& name)
{
	D3D12_RANGE range{ 0, 1 };

	auto hr = descMane.GetRsc(rsc[name].id)->Map(0, &range, reinterpret_cast<void**>(&rsc[name].data));
	if (FAILED(hr))
	{
		return hr;
	}

	return hr;
}

// CBV�̐���
void DFT::CreateCbv(const std::string & name)
{
	CreateCbvRsc(name);
	CreateCbvView(name);
	Map(name);
}

// UAV�̐���
void DFT::CreateUav(const std::string & name)
{
	CreateUavRsc(name);
	CreateUavView(name);
	Map(name);
}

// ������
void DFT::Init(void)
{
	CreateHeap();
	CreateCbv("b0");
	CreateUav("u0");
}

// ����
void DFT::UpData(void)
{
	WaitForSingleObject(sound->GetCallback()->handle, INFINITE);

	memcpy(rsc["b0"].data, SoundLoader::Get().GetData(sound->GetName())->at(waveIndex).data(), sizeof(float) * SoundLoader::Get().GetData(sound->GetName())->at(waveIndex).size());
	memset(rsc["u0"].data, 0, DATA_MAX);

	com->GetList()->Reset(nullptr);

	com->GetList()->GetList()->SetComputeRootSignature(root.lock()->Get());
	com->GetList()->SetPipe(pipe.lock()->Get());

	auto heap = descMane.GetHeap(heapID);
	auto handle = heap->GetGPUDescriptorHandleForHeapStart();
	com->GetList()->GetList()->SetDescriptorHeaps(1, &heap);
	handle.ptr += dev.lock()->GetDev()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * rsc["b0"].id;
	com->GetList()->GetList()->SetComputeRootDescriptorTable(0, handle);
	handle.ptr += dev.lock()->GetDev()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * rsc["u0"].id;
	com->GetList()->GetList()->SetComputeRootDescriptorTable(1, handle);

	com->GetList()->GetList()->Dispatch(1024, 1, 1);

	com->GetList()->Close();
	ID3D12CommandList* list = com->GetList()->GetList();
	com->GetQueue()->Execute(&list, 1);

	fence->Wait();

	std::vector<float>wave(rsc["u0"].data, rsc["u0"].data + SoundLoader::Get().GetData(sound->GetName())->at(waveIndex).size());

	if (SoundLoader::Get().GetFlag(sound->GetName()) == false)
	{
		return;
	}

	waveIndex = (waveIndex + 1 >= SoundLoader::Get().GetData(sound->GetName())->size()) ? 0 : ++waveIndex;
}
