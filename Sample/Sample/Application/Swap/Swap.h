#pragma once
#include <memory>

struct IDXGISwapChain4;
class Device;
class Queue;

class Swap
{
public:
	// �R���X�g���N�^
	Swap(std::weak_ptr<Device>dev, std::weak_ptr<Queue>queue);
	// �f�X�g���N�^
	~Swap();

	// ���\�𔽓]
	void Present(void);

	// �X���b�v�`�F�C���̎擾
	IDXGISwapChain4* Get(void) const {
		return swap;
	}

private:
	// �X���b�v�`�F�C���̐���
	long Create(void);


	// �f�o�C�X
	std::weak_ptr<Device>dev;

	// �R�}���h�L���[
	std::weak_ptr<Queue>queue;

	// �X���b�v�`�F�C��
	IDXGISwapChain4* swap;
};
