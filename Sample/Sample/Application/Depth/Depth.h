#pragma once
#include <memory>

struct ID3D12GraphicsCommandList;
struct ID3D12DescriptorHeap;
struct ID3D12Resource;
class Device;

class Depth
{
public:
	// �R���X�g���N�^
	Depth(std::weak_ptr<Device>dev);
	// �f�X�g���N�^
	~Depth();

	// �f�v�X�̃Z�b�g�E�N���A
	void SetDepth(ID3D12GraphicsCommandList* list);

	// �q�[�v�̎擾
	ID3D12DescriptorHeap* GetHeap(void) const {
		return heap;
	}
	// ���\�[�X�̎擾
	ID3D12Resource* GetRsc(void) const {
		return rsc;
	}

private:
	// �q�[�v�̐���
	long CreateHeap(void);

	// ���\�[�X�̐���
	long CreateRsc(void);

	// �r���[�̐���
	void CreateView(void);

	// ������
	void Init(void);


	// �f�o�C�X
	std::weak_ptr<Device>dev;

	// �q�[�v
	ID3D12DescriptorHeap* heap;

	// ���\�[�X
	ID3D12Resource* rsc;
};
