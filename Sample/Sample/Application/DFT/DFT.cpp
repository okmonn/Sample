#include "DFT.h"
#include "../../Audio/SoundLoader.h"
#include "../DescriptorMane/DescriptorMane.h"
#include "../../Audio/XAudio2.h"
#include "../../Audio/VoiceCallback.h"
#include "../../Audio/Sound.h"
#include "../Device/Device.h"
#include "../Queue/Queue.h"
#include "../List/List.h"
#include "../Command/Command.h"
#include "../Fence/Fence.h"
#include "../Root/Root.h"
#include "../Root/RootCompute.h"
#include "../Pipe/Pipe.h"
#include "../Pipe/PipeCompute.h"
#include "../Release.h"
#include <DirectXMath.h>

// リソース数
#define RSC_MAX 2

// データ数
#define DATA_MAX 1764

// 頂点
namespace dft {
	struct Vertex {
		DirectX::XMFLOAT3 pos;
	};

	std::vector<Vertex>vertex;
}

// コンストラクタ
DFT::DFT(std::weak_ptr<Device>dev) : 
	descMane(DescriptorMane::Get()), dev(dev), nomber(0), index(0), heap(0)
{
	rsc.clear();

	audio = std::make_shared<XAudio2>();
	sound = std::make_unique<Sound>(audio);
	com = std::make_unique<Command>(this->dev, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_COMPUTE);
	fen = std::make_unique<Fence>(this->dev, com->GetQueue());

	CreateHeap();
	Init();
}

// デストラクタ
DFT::~DFT()
{
}

// ヒープの生成
long DFT::CreateHeap(void)
{
	return descMane.CreateHeap(dev, heap, D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, RSC_MAX);
}

// リソースの生成
long DFT::CreateCbvRsc(const std::string & name, const unsigned int & size)
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

	rsc[name].id = nomber++;

	return descMane.CreateRsc(dev, rsc[name].id, prop, desc);
}

// UAVリソースの生成
long DFT::CreateUavRsc(const std::string & name, const unsigned int & size)
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

	rsc[name].id = nomber++;

	return descMane.CreateRsc(dev, rsc[name].id, prop, desc, nullptr, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
}

// CBVの生成
void DFT::CreateCbv(const std::string & name, const unsigned int & size)
{
	D3D12_CONSTANT_BUFFER_VIEW_DESC desc{};
	desc.BufferLocation = descMane.GetRsc(rsc[name].id)->GetGPUVirtualAddress();
	desc.SizeInBytes    = (size + 0xff) &~0xff;

	auto handle = descMane.GetHeap(heap)->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += dev.lock()->GetDev()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * rsc[name].id;

	dev.lock()->GetDev()->CreateConstantBufferView(&desc, handle);
}

// UAVの生成
void DFT::CreateUav(const std::string & name, const unsigned int & stride, const unsigned int & num)
{
	D3D12_UNORDERED_ACCESS_VIEW_DESC desc{};
	desc.ViewDimension              = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_BUFFER;
	desc.Format                     = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	desc.Buffer.NumElements         = num;
	desc.Buffer.StructureByteStride = stride;

	auto handle = descMane.GetHeap(heap)->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += dev.lock()->GetDev()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * rsc[name].id;

	dev.lock()->GetDev()->CreateUnorderedAccessView(descMane.GetRsc(rsc[name].id), nullptr, &desc, handle);
}

// マップ
long DFT::Map(const std::string & name)
{
	D3D12_RANGE range{0, 1};

	auto hr = descMane.GetRsc(rsc[name].id)->Map(0, &range, reinterpret_cast<void**>(&rsc[name].data));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nリソースのマップ：失敗\n"));
	}

	return hr;
}

// CBVの生成
void DFT::Cbv(const std::string & name, const unsigned int & size)
{
	CreateCbvRsc(name, size);
	CreateCbv(name, size);
	Map(name);
}

// UAVの生成
void DFT::Uav(const std::string & name, const unsigned int & stride, const unsigned int & num)
{
	CreateUavRsc(name, stride * num);
	CreateUav(name, stride, num);
	Map(name);
}

// 頂点リソースの生成
long DFT::CreateVertex(void)
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
	desc.Width            = sizeof(dft::Vertex) * DATA_MAX * 2;

	return descMane.CreateRsc(dev, rsc["vertex"].id, prop, desc);
}

// 初期化
void DFT::Init(void)
{
	Cbv("b0", sizeof(float) * DATA_MAX);
	Uav("u0", sizeof(float), DATA_MAX);

	CreateVertex();
	Map("vertex");
	dft::vertex.resize(DATA_MAX * 2);

	for (unsigned int i = 0; i < dft::vertex.size(); i += 2)
	{
		dft::vertex[i].pos.x     = i * (2.0f / (float)DATA_MAX) - 1.0f;
		dft::vertex[i + 1].pos.x = i * (2.0f / (float)DATA_MAX) - 1.0f;
	}
}

// コンピュート用ルートシグネチャ・パイプラインのセット
void DFT::SetCompute(std::weak_ptr<RootCompute> root, std::weak_ptr<PipeCompute> pipe)
{
	cRoot = root;
	cPipe = pipe;
}

// グラフィックス用ルートシグネチャ・パイプラインのセット
void DFT::SetGraphics(std::weak_ptr<Root> root, std::weak_ptr<Pipe> pipe)
{
	this->root = root;
	this->pipe = pipe;
}

// 音声読み込み
void DFT::Load(const std::string & fileName)
{
	sound->Load(fileName);
	sound->Play(false);
}

// 実行
void DFT::Execution(void)
{
	if (sound->GetEnd() == true)
	{
		return;
	}

	WaitForSingleObject(sound->GetCallback()->handle, INFINITE);

	memcpy(rsc["b0"].data, SoundLoader::Get().GetData(sound->GetName())->at(index).data(), sizeof(float) * SoundLoader::Get().GetData(sound->GetName())->at(index).size());
	memset(rsc["u0"].data, 0, sizeof(float) * SoundLoader::Get().GetData(sound->GetName())->at(index).size());

	com->GetList()->Reset(nullptr);

	com->GetList()->GetList()->SetComputeRootSignature(cRoot.lock()->Get());
	com->GetList()->SetPipe(cPipe.lock()->Get());

	auto h = descMane.GetHeap(heap);
	auto handle = h->GetGPUDescriptorHandleForHeapStart();
	com->GetList()->GetList()->SetDescriptorHeaps(1, &h);
	handle.ptr += dev.lock()->GetDev()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * rsc["b0"].id;
	com->GetList()->GetList()->SetComputeRootDescriptorTable(0, handle);
	handle.ptr += dev.lock()->GetDev()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * rsc["u0"].id;
	com->GetList()->GetList()->SetComputeRootDescriptorTable(1, handle);

	com->GetList()->GetList()->Dispatch(SoundLoader::Get().GetData(sound->GetName())->at(index).size(), 1, 1);

	com->GetList()->Close();
	ID3D12CommandList* list = com->GetList()->GetList();
	com->GetQueue()->Execute(&list, 1);

	fen->Wait();

	wave.assign(rsc["u0"].data, rsc["u0"].data + SoundLoader::Get().GetData(sound->GetName())->at(index).size());

	index = (index + 1 >= SoundLoader::Get().GetData(sound->GetName())->size()) ? 0 : ++index;
}

// 描画
void DFT::Draw(std::weak_ptr<List> list)
{
	for (unsigned int i = 0; i < wave.size(); ++i)
	{
		dft::vertex[i * 2].pos.y     = 0.0f;
		dft::vertex[i * 2 + 1].pos.y = wave[i] * (32768.0f / 2);
	}

	memcpy(rsc["vertex"].data, dft::vertex.data(), sizeof(dft::Vertex) * dft::vertex.size());

	list.lock()->SetRoot(root.lock()->Get());
	list.lock()->SetPipe(pipe.lock()->Get());

	D3D12_VERTEX_BUFFER_VIEW view{};
	view.BufferLocation = descMane.GetRsc(rsc["vertex"].id)->GetGPUVirtualAddress();
	view.SizeInBytes    = sizeof(dft::Vertex) * DATA_MAX;
	view.StrideInBytes  = sizeof(dft::Vertex);
	list.lock()->GetList()->IASetVertexBuffers(0, 1, &view);

	list.lock()->GetList()->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_LINELIST);

	list.lock()->GetList()->DrawInstanced(DATA_MAX, 1, 0, 0);
}
