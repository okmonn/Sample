#pragma once
#include <memory>

class DescriptorMane;
class Device;
class List;
class Command;
class RootCompute;
class PipeCompute;

class Compute
{
public:
	// �R���X�g���N�^
	Compute(std::weak_ptr<Device>dev, std::weak_ptr<RootCompute>root, std::weak_ptr<PipeCompute>pipe);
	// �f�X�g���N�^
	~Compute();

	// ���[�g�V�O�l�`���R���s���[�g�̃Z�b�g
	void SetRoot(void);

	// �p�C�v���C���R���s���[�g�̃Z�b�g
	void SetPipe(void);

	// ���s
	void Do(std::weak_ptr<List>list);

private:
	// ���\�[�X�̐���
	long CreateRsc(void);

	// UAV�r���[�̐���
	void CreateView(void);

	// �}�b�v
	long Map(void);


	// �q�[�v�E���\�[�X�}�l�W���[
	DescriptorMane& descMane;

	// �f�o�C�X
	std::weak_ptr<Device>dev;

	// �R�}���h
	std::unique_ptr<Command>com;

	// ���[�g�V�O�l�`��
	std::weak_ptr<RootCompute>root;

	// �p�C�v���C��
	std::weak_ptr<PipeCompute>pipe;

	// �q�[�v�E���\�[�XID
	int id;

	int cId;

	// �f�[�^
	float* data;
};
