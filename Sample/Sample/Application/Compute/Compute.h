#pragma once
#include <memory>

class DescriptorMane;
class Device;
class Command;
class Fence;
class RootCompute;
class PipeCompute;

class Compute
{
public:
	// �R���X�g���N�^
	Compute(std::weak_ptr<Device>dev, std::weak_ptr<RootCompute>root, std::weak_ptr<PipeCompute>pipe);
	// �f�X�g���N�^
	~Compute();

	// ���s
	void Execution(void);

private:
	// UAV�p���\�[�X�̐���
	long CreateUavRsc(void);

	// UAV�r���[�̐���
	void CreateView(void);

	// �R�s�[���\�[�X�̐���
	long CreateCopyRsc(void);

	// ������
	void Init(void);

	// �}�b�v
	long Map(void);


	// �f�B�X�N���v�^�}�l�[�W���[
	DescriptorMane& descMane;

	// �f�o�C�X
	std::weak_ptr<Device>dev;

	// �R�}���h
	std::unique_ptr<Command>com;

	// �t�F���X
	std::unique_ptr<Fence>fence;

	// �R���s���[�g�p���[�g�V�O�l�`��
	std::weak_ptr<RootCompute>root;

	// �R���s���[�g�p�p�C�v���C��
	std::weak_ptr<PipeCompute>pipe;

	// UAV��ID
	int uav;

	// �R�s�[��ID
	int copy;

	// ��M�p�f�[�^
	float* data;
};
