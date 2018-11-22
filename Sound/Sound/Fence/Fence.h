#pragma once
#include <memory>

struct ID3D12Fence;
class Device;
class Queue;

class Fence
{
public:
	// �R���X�g���N�^
	Fence(std::weak_ptr<Device>dev, std::weak_ptr<Queue>queue);
	// �f�X�g���N�^
	~Fence();

	// �ҋ@
	void Wait(void);

	// �t�F���X�擾
	ID3D12Fence* Get(void) const {
		return fence;
	}

private:
	// �t�F���X�̐���
	long CreateFence(void);


	// �f�o�C�X
	std::weak_ptr<Device>dev;

	// �L���[
	std::weak_ptr<Queue>queue;

	// �t�F���X
	ID3D12Fence* fence;

	// �n���h��
	void* handle;

	// �t�F���X�J�E���g
	long long cnt;
};