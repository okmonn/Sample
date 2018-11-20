#pragma once
#include <map>
#include <string>
#include <vector>
#include <memory>

class DescriptorMane;
class XAudio2;
class Sound;
class Device;
class List;
class Command;
class Fence;
class Root;
class RootCompute;
class Pipe;
class PipeCompute;

class DFT
{
	struct Data {
		int id;
		float* data;
	};

public:
	// �R���X�g���N�^
	DFT(std::weak_ptr<Device>dev);
	// �f�X�g���N�^
	~DFT();

	// �R���s���[�g�p���[�g�V�O�l�`���E�p�C�v���C���̃Z�b�g
	void SetCompute(std::weak_ptr<RootCompute>root, std::weak_ptr<PipeCompute>pipe);

	// �O���t�B�b�N�X�p���[�g�V�O�l�`���E�p�C�v���C���̃Z�b�g
	void SetGraphics(std::weak_ptr<Root>root, std::weak_ptr<Pipe>pipe);

	// �����ǂݍ���
	void Load(const std::string& fileName);

	// ���s
	void Execution(void);

	// �`��
	void Draw(std::weak_ptr<List>list);

private:
	// �q�[�v�̐���
	long CreateHeap(void);

	// CBV���\�[�X�̐���
	long CreateCbvRsc(const std::string& name, const unsigned int& size);

	// UAV���\�[�X�̐���
	long CreateUavRsc(const std::string& name, const unsigned int& size);

	// CBV�̐���
	void CreateCbv(const std::string& name, const unsigned int& size);

	// UAV�̐���
	void CreateUav(const std::string& name, const unsigned int& stride, const unsigned int& num);

	// �}�b�v
	long Map(const std::string& name);

	// CBV�̐���
	void Cbv(const std::string& name, const unsigned int& size);

	// UAV�̐���
	void Uav(const std::string& name, const unsigned int& stride, const unsigned int& num);

	// ���_���\�[�X�̐���
	long CreateVertex(void);

	// ������
	void Init(void);


	// �f�B�X�N���v�^�}�l�[�W���[
	DescriptorMane& descMane;

	// XAudio2
	std::shared_ptr<XAudio2>audio;

	// �T�E���h
	std::unique_ptr<Sound>sound;

	// �f�o�C�X
	std::weak_ptr<Device>dev;

	// �R�}���h
	std::unique_ptr<Command>com;

	// �t�F���X
	std::unique_ptr<Fence>fen;

	// ���[�g�V�O�l�`���E
	std::weak_ptr<Root>root;

	// �R���s���[�g���[�g�V�O�l�`��
	std::weak_ptr<RootCompute>cRoot;

	// �p�C�v���C��
	std::weak_ptr<Pipe>pipe;

	// �R���s���[�g�p�C�v���C��
	std::weak_ptr<PipeCompute>cPipe;

	// ���\�[�X�ԍ�
	int nomber;

	// �g�`�z��ԍ�
	unsigned int index;

	// �q�[�vID
	int heap;

	// ���\�[�XID
	std::map<std::string, Data>rsc;

	// �g�`�ϊ��f�[�^
	std::vector<float>wave;
};
