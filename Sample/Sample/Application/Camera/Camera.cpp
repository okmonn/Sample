#include "Camera.h"
#include "../DescriptorMane/DescriptorMane.h"
#include "../Device/Device.h"
#include "../List/List.h"
#include "../Release.h"

// コンストラクタ
Camera::Camera(std::weak_ptr<Device>dev) :
	descMane(DescriptorMane::Get()), dev(dev), data(nullptr)
{
	list = std::make_unique<List>(dev, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_BUNDLE);

	Init();
}

// デストラクタ
Camera::~Camera()
{
	UnMap(descMane.GetRsc(id));
}

// リソースの生成
long Camera::CreateRsc(void)
{
	if (FAILED(descMane.CreateHeap(dev, id, D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE)))
	{
		return S_FALSE;
	}

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
	desc.Width            = (sizeof(VP) + 0xff) &~0xff;

	return  descMane.CreateRsc(dev, id, prop, desc);
}

// 定数バッファビューの生成
void Camera::CreateConView(void)
{
	D3D12_CONSTANT_BUFFER_VIEW_DESC desc{};
	desc.BufferLocation = descMane.GetRsc(id)->GetGPUVirtualAddress();
	desc.SizeInBytes = (sizeof(VP) + 0xff) &~0xff;

	auto handle = descMane.GetHeap(id)->GetCPUDescriptorHandleForHeapStart();
	dev.lock()->GetDev()->CreateConstantBufferView(&desc, handle);
}

// マップ
long Camera::Map(void)
{
	auto hr = descMane.GetRsc(id)->Map(0, nullptr, (void**)(&data));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nカメラの定数バッファのマップ：失敗\n"));
	}

	return  hr;
}

// バンドルのセット
void Camera::SetBundle(void)
{
	list->Reset(nullptr);

	auto heap = descMane.GetHeap(id);
	list->GetList()->SetDescriptorHeaps(1, &heap);
	list->GetList()->SetGraphicsRootDescriptorTable(0, heap->GetGPUDescriptorHandleForHeapStart());

	list->Close();
}

// 初期化
void Camera::Init(void)
{
	CreateRsc();
	CreateConView();
	Map();
}
