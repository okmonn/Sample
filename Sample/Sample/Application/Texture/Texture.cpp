#include "Texture.h"
#include "../DescriptorMane/DescriptorMane.h"
#include "TextureLoader.h"
#include "../Device/Device.h"
#include "../List/List.h"
#include "../Root/Root.h"
#include "../Pipe/Pipe.h"
#include "../Release.h"

namespace {
	tex::Vertex vertex[] = {
		{{-1.0f,  1.0f, 0.0f}, {0.0f, 0.0f}},
		{{ 1.0f,  1.0f, 0.0f}, {1.0f, 0.0f}}, 
		{{-1.0f, -1.0f, 0.0f}, {0.0f, 1.0f}}, 
		{{ 1.0f, -1.0f, 0.0f}, {1.0f, 1.0f}}
	};
}

// コンストラクタ
Texture::Texture(std::weak_ptr<Device>dev, std::weak_ptr<Root>root, std::weak_ptr<Pipe>pipe) :
	descMane(DescriptorMane::Get()), loader(TextureLoad::Get()), dev(dev), root(root), pipe(pipe)
{
	tex.clear();
}

// デストラクタ
Texture::~Texture()
{
}

// シェーダーリソースビューの生成
void Texture::CreateShaderView(int * i)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC desc{};
	desc.Format                    = tex[i].rsc->GetDesc().Format;
	desc.ViewDimension             = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipLevels       = 1;
	desc.Texture2D.MostDetailedMip = 0;
	desc.Shader4ComponentMapping   = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	dev.lock()->GetDev()->CreateShaderResourceView(tex[i].rsc, &desc, descMane.GetHeap(*i)->GetCPUDescriptorHandleForHeapStart());
}

// サブリソースに書き込み
long Texture::WriteSub(int * i)
{
	D3D12_BOX box{};
	box.back   = 1;
	box.bottom = tex[i].rsc->GetDesc().Height;
	box.front  = 0;
	box.left   = 0;
	box.right  = static_cast<UINT>(tex[i].rsc->GetDesc().Width);
	box.top    = 0;

	auto hr = tex[i].rsc->WriteToSubresource(0, &box, &tex[i].decode[0], tex[i].sub.lock()->RowPitch, tex[i].sub.lock()->SlicePitch);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nサブリソースの更新：失敗"));
	}

	return hr;
}

// 頂点リソースの生成
long Texture::CreateVertexRsc(int * i)
{
	D3D12_HEAP_PROPERTIES prop{};
	prop.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.CreationNodeMask     = 1;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	prop.Type                 = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD;
	prop.VisibleNodeMask      = 1;

	D3D12_RESOURCE_DESC desc{};
	desc.Alignment          = 0;
	desc.DepthOrArraySize   = 1;
	desc.Dimension          = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Flags              = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;
	desc.Format             = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	desc.Height             = 1;
	desc.Layout             = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.MipLevels          = 1;
	desc.SampleDesc.Count   = 1;
	desc.SampleDesc.Quality = 0;
	desc.Width              = sizeof(vertex);

	return descMane.CreateRsc(dev, tex[i].vRsc, prop, desc);
}

// 頂点マップ
long Texture::Map(int * i)
{
	auto hr = descMane.GetRsc(tex[i].vRsc)->Map(0, nullptr, reinterpret_cast<void**>(&tex[i].data));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nテクスチャの頂点マッピング：失敗\n"));
		return hr;
	}
	//頂点データのコピー
	memcpy(tex[i].data, &vertex[0], sizeof(vertex));

	descMane.GetRsc(tex[i].vRsc)->Unmap(0, nullptr);

	return hr;
}

// バンドルのセット
void Texture::SetBundle(int * i)
{
	tex[i].list->Reset(pipe.lock()->Get());

	tex[i].list->SetRoot(root.lock()->Get());
	tex[i].list->SetPipe(pipe.lock()->Get());

	D3D12_VERTEX_BUFFER_VIEW view{};
	view.BufferLocation = descMane.GetRsc(tex[i].vRsc)->GetGPUVirtualAddress();
	view.SizeInBytes    = sizeof(vertex);
	view.StrideInBytes  = sizeof(tex::Vertex);
	tex[i].list->GetList()->IASetVertexBuffers(0, 1, &view);

	tex[i].list->GetList()->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	auto heap = descMane.GetHeap(*i);
	tex[i].list->GetList()->SetDescriptorHeaps(1, &heap);
	tex[i].list->GetList()->SetGraphicsRootDescriptorTable(0, descMane.GetHeap(*i)->GetGPUDescriptorHandleForHeapStart());
	tex[i].list->GetList()->DrawInstanced(_countof(vertex), 1, 0, 0);

	tex[i].list->Close();
}

// 読み込み
void Texture::Load(const std::string & fileName, int & i)
{
	if (FAILED(loader.Load(dev, fileName)))
	{
		return;
	}

	tex[&i].rsc    = loader.GetRsc(fileName);
	tex[&i].decode = loader.GetDecode(fileName);
	tex[&i].sub    = loader.GetSub(fileName);
	tex[&i].list   = std::make_unique<List>(dev, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_BUNDLE);

	descMane.CreateHeap(dev, i, D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);
	CreateShaderView(&i);
	WriteSub(&i);
	CreateVertexRsc(&i);
	Map(&i);
	SetBundle(&i);
}

// 描画
void Texture::Draw(ID3D12GraphicsCommandList * list, int & i)
{
	auto heap = descMane.GetHeap(i);
	list->SetDescriptorHeaps(1, &heap);
	list->ExecuteBundle(tex[&i].list->GetList());
}

// 削除
void Texture::Delete(int & i)
{
	if (tex.find(&i) != tex.end())
	{
		tex.erase(tex.find(&i));
	}
}
