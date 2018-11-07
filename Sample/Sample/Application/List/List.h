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
	List(std::weak_ptr<Device>dev, const D3D12_COMMAND_LIST_TYPE& type);
	// �f�X�g���N�^
	~List();

	// �R�}���h�A���P�[�^�̎擾
	ID3D12CommandAllocator* GetAllo(void) const {
		return allo;
	}
	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* GetList(void) const {
		return list;
	}

private:
	// �R�}���h�A���P�[�^�̐���
	long CreateAllo(const D3D12_COMMAND_LIST_TYPE& type);

	// �R�}���h���X�g�̐���
	long CreateList(const D3D12_COMMAND_LIST_TYPE& type);

	// ������
	void Init(const D3D12_COMMAND_LIST_TYPE & type);


	// �f�o�C�X
	std::weak_ptr<Device>dev;

	// �R�}���h�A���P�[�^
	ID3D12CommandAllocator* allo;

	// �R�}���h���X�g
	ID3D12GraphicsCommandList* list;
};
