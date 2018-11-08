#pragma once
#include <map>
#include <memory>

enum D3D12_DESCRIPTOR_HEAP_FLAGS : int;
enum D3D12_DESCRIPTOR_HEAP_TYPE : int;
enum D3D12_RESOURCE_STATES:int;
struct D3D12_HEAP_PROPERTIES;
struct D3D12_RESOURCE_DESC;
struct ID3D12DescriptorHeap;
struct ID3D12Resource;
class Device;

class DescriptorMane
{
public:
	// �R���X�g���N�^
	DescriptorMane(std::weak_ptr<Device>dev);
	// �f�X�g���N�^
	~DescriptorMane();

	// �q�[�v�̐���
	long CreateHeap(int& i, const D3D12_DESCRIPTOR_HEAP_FLAGS& flag = static_cast<D3D12_DESCRIPTOR_HEAP_FLAGS>(0),
		const unsigned int& num = 1, const D3D12_DESCRIPTOR_HEAP_TYPE& type = static_cast<D3D12_DESCRIPTOR_HEAP_TYPE>(0));

	// ���\�[�X�̐���
	long CreateRsc(int& i, const D3D12_HEAP_PROPERTIES& prop, const D3D12_RESOURCE_DESC& desc,
		const D3D12_RESOURCE_STATES& state = static_cast<D3D12_RESOURCE_STATES>(2755));

	// �q�[�v�̎擾
	std::shared_ptr<ID3D12DescriptorHeap*>GetHeap(int& i) {
		return heap[&i];
	}
	// ���\�[�X�̎擾
	std::shared_ptr<ID3D12Resource*>GetRsc(int& i) {
		return rsc[&i];
	}

private:
	// �f�o�C�X
	std::weak_ptr<Device>dev;

	// �q�[�v
	std::map<int*, std::shared_ptr<ID3D12DescriptorHeap*>>heap;

	// ���\�[�X
	std::map<int*, std::shared_ptr<ID3D12Resource*>>rsc;
};
