#pragma once
#include <memory>

enum D3D12_COMMAND_LIST_TYPE : int;
struct ID3D12CommandAllocator;
struct ID3D12GraphicsCommandList;
class Device;

class List
{
public:
	// �R���X�g���N�^
	List(std::weak_ptr<Device>dev, const D3D12_COMMAND_LIST_TYPE& type = (D3D12_COMMAND_LIST_TYPE)0);
	// �f�X�g���N�^
	~List();

	// �A���P�[�^�̎擾
	ID3D12CommandAllocator* GetAllo(void) const {
		return allo;
	}
	// ���X�g�̎擾
	ID3D12GraphicsCommandList* GetList(void) const {
		return list;
	}

private:
	// �A���P�[�^�̐���
	long CreateAllo(const D3D12_COMMAND_LIST_TYPE& type);

	// ���X�g�̐���
	long CreateList(const D3D12_COMMAND_LIST_TYPE& type);


	// �f�o�C�X
	std::weak_ptr<Device>dev;

	// �A���P�[�^
	ID3D12CommandAllocator* allo;

	// ���X�g
	ID3D12GraphicsCommandList* list;
};
