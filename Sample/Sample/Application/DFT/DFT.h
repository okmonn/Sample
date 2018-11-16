#pragma once
#include <map>
#include <string>
#include <vector>
#include <memory>

class SoundLoader;
class DescriptorMane;
class Device;
class Command;
class Fence;
class RootCompute;
class PipeCompute;

class DFT
{
	struct Info {
		int id;
		float* data;
	};
public:
	// �R���X�g���N�^
	DFT(std::weak_ptr<Device>dev, std::weak_ptr<RootCompute>root, std::weak_ptr<PipeCompute>pipe);
	// �f�X�g���N�^
	~DFT();

	// ����
	void UpData(void);

private:
	// �q�[�v�̐���
	long CreateHeap(void);

	// CBV���\�[�X�̐���
	long CreateCbvRsc(const std::string& name);

	// CBV�r���[�̐���
	void CreateCbvView(const std::string& name);

	// UAV���\�[�X�̐���
	long CreateUavRsc(const std::string& name);

	// UAV�r���[�̐���
	void CreateUavView(const std::string& name);

	// ���\�[�X�̃}�b�v
	long Map(const std::string& name);

	// CBV�̐���
	void CreateCbv(const std::string& name);

	// UAV�̐���
	void CreateUav(const std::string& name);

	// ������
	void Init(void);


	// �T�E���h���[�_�[
	SoundLoader& loader;

	// �q�[�v�E���\�[�X�}�l�[�W���[
	DescriptorMane& descMane;

	// �f�o�C�X
	std::weak_ptr<Device>dev;

	// �R�}���h
	std::unique_ptr<Command>com;

	// �t�F���X
	std::unique_ptr<Fence>fence;

	// �R���s���[�g���[�g�V�O�l�`��
	std::weak_ptr<RootCompute>root;

	// �R���s���[�g�p�C�v���C��
	std::weak_ptr<PipeCompute>pipe;

	// �q�[�vID
	int heapID;

	// ���\�[�X�ԍ�
	int index;

	// ���\�[�XID
	std::map<std::string, Info>rsc;

	// 
	std::weak_ptr<std::vector<std::vector<float>>>data;
};
