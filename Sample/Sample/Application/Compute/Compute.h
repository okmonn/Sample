#pragma once
#include <memory>

class DescriptorMane;
class Device;
class Command;

class Compute
{
public:
	// �R���X�g���N�^
	Compute(std::weak_ptr<Device>dev);
	// �f�X�g���N�^
	~Compute();

private:
	// �q�[�v�E���\�[�X�}�l�W���[
	DescriptorMane& descMane;

	// �f�o�C�X
	std::weak_ptr<Device>dev;
};
