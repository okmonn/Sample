#include "Camera.h"
#include "../DescriptorMane/DescriptorMane.h"
#include "../Device/Device.h"
#include "../List/List.h"
#include "../Release.h"

// �R���X�g���N�^
Camera::Camera(std::weak_ptr<Device>dev) :
	descMane(DescriptorMane::Get()), dev(dev), data(nullptr)
{
	list = std::make_unique<List>(dev, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_BUNDLE);

	Init();
}

// �f�X�g���N�^
Camera::~Camera()
{
	UnMap(descMane.GetRsc(id));
}

// ���\�[�X�̐���
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

// �萔�o�b�t�@�r���[�̐���
void Camera::CreateConView(void)
{
	D3D12_CONSTANT_BUFFER_VIEW_DESC desc{};
	desc.BufferLocation = descMane.GetRsc(id)->GetGPUVirtualAddress();
	desc.SizeInBytes = (sizeof(VP) + 0xff) &~0xff;

	auto handle = descMane.GetHeap(id)->GetCPUDescriptorHandleForHeapStart();
	dev.lock()->GetDev()->CreateConstantBufferView(&desc, handle);
}

// �}�b�v
long Camera::Map(void)
{
	auto hr = descMane.GetRsc(id)->Map(0, nullptr, (void**)(&data));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n�J�����̒萔�o�b�t�@�̃}�b�v�F���s\n"));
	}

	return  hr;
}

// �o���h���̃Z�b�g
void Camera::SetBundle(void)
{
	list->Reset(nullptr);

	auto heap = descMane.GetHeap(id);
	list->GetList()->SetDescriptorHeaps(1, &heap);
	list->GetList()->SetGraphicsRootDescriptorTable(0, heap->GetGPUDescriptorHandleForHeapStart());

	list->Close();
}

// ������
void Camera::Init(void)
{
	CreateRsc();
	CreateConView();
	Map();
}
