#pragma once
#include <memory>

enum D3D12_COMMAND_LIST_TYPE : int;
enum D3D12_RESOURCE_STATES : int;
struct ID3D12CommandAllocator;
struct ID3D12GraphicsCommandList;
struct ID3D12Resource;
struct ID3D12RootSignature;
struct ID3D12PipelineState;
class Device;

class List
{
public:
	// �R���X�g���N�^
	List(std::weak_ptr<Device>dev, const D3D12_COMMAND_LIST_TYPE& type);
	// �f�X�g���N�^
	~List();

	// �R�}���h���X�g�����
	void Close(void);

	// ���Z�b�g
	void Reset(ID3D12PipelineState* pipe);

	// ���[�g�V�O�l�`���̃Z�b�g
	void SetRoot(ID3D12RootSignature* root);

	// �p�C�v���C���̃Z�b�g
	void SetPipe(ID3D12PipelineState* pipe);

	// �r���[�|�[�g�̃Z�b�g
	void SetViewport(void);

	// �V�U�[�̃Z�b�g
	void SetScissor(void);

	// �o���A�̃Z�b�g
	void SetBarrier(const D3D12_RESOURCE_STATES& befor, const D3D12_RESOURCE_STATES& affter, ID3D12Resource* rsc);

	// �R���s���[�g�o���A�̃Z�b�g
	void SetComputeBarrier(ID3D12Resource * rsc);

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
