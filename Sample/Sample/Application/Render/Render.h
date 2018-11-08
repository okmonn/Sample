#pragma once
#include <vector>
#include <memory>

struct ID3D12GraphicsCommandList;
struct ID3D12DescriptorHeap;
struct ID3D12Resource;
class Device;
class Swap;

class Render
{
public:
	// �R���X�g���N�^
	Render(std::weak_ptr<Device>dev, std::weak_ptr<Swap>swap);
	// �f�X�g���N�^
	~Render();

	// �����_�[�^�[�Q�b�g�̃Z�b�g
	void SetRender(ID3D12GraphicsCommandList* list, ID3D12DescriptorHeap* depth, const float& color);

	// �q�[�v�̎擾
	ID3D12DescriptorHeap* GetHeap(void) const {
		return heap;
	}
	// ���\�[�X�̎擾
	ID3D12Resource* GetRsc(const unsigned int& i) const {
		return rsc[i];
	}

private:
	// �q�[�v�̐���
	long CreateHeap(void);

	// ���\�[�X�ƃ����_�[�^�[�Q�b�g�r���[�̐���
	long CreateRscView(void);

	// ������
	void Init(void);


	// �f�o�C�X
	std::weak_ptr<Device>dev;

	// �X���b�v�`�F�C��
	std::weak_ptr<Swap>swap;

	// �q�[�v
	ID3D12DescriptorHeap* heap;

	// ���\�[�X
	std::vector<ID3D12Resource*>rsc;
};
