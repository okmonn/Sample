#pragma once
#include <map>
#include <string>
#include <vector>
#include <memory>

struct ID3D12DescriptorHeap;
struct ID3D12Resource;
class Queue;
class List;
class Device;
class Root;
class Pipe;

class Effector
{
	struct Info {
		//���\�[�X
		ID3D12Resource* rsc;
		//���M�f�[�^
		float* data;
		//�ԍ�
		int index;
	};

	// �p�����[�^
	struct Param {
		//������
		float attenuation;
		//�x������
		float time;
		//���[�v��
		int loop;
		//�T���v�����O���g��
		int sample;
	};

public:
	// �R���X�g���N�^
	Effector(std::weak_ptr<Device>dev, std::weak_ptr<Root>root, std::weak_ptr<Pipe>pipe);
	// �f�X�g���N�^
	~Effector();

	// ���s
	void Execution(const std::vector<float>& wave, std::vector<float>& adaptation);

private:
	// �q�[�v�̐���
	long CreateHeap(void);

	// CBV���\�[�X�̐���
	long CreateCbvRsc(const std::string& name, const unsigned int& size);

	// UAV���\�[�X�̐���
	long CreateUavRsc(const std::string& name, const unsigned int& size);

	// CBV�̐���
	void CreateConstantView(const std::string& name, const unsigned int& size);

	// UAV�̐���
	void CreateUnorderView(const std::string& name, const unsigned int& stride, const unsigned int& num);

	// �}�b�v
	long Map(const std::string& name);

	// CBV�̐���
	void CBV(const std::string& name, const unsigned int& size);

	// UAV�̐���
	void UAV(const std::string& name, const unsigned int& stride, const unsigned int& num);

	// ������
	void Init(void);


	// �f�o�C�X
	std::weak_ptr<Device>dev;

	// �L���[
	std::unique_ptr<Queue>queue;

	// ���X�g
	std::unique_ptr<List>list;

	// ���[�g
	std::weak_ptr<Root>root;

	// �p�C�v
	std::weak_ptr<Pipe>pipe;

	// �q�[�v
	ID3D12DescriptorHeap* heap;

	// ���\�[�X�ԍ�
	int index;

	// �p�����[�^
	Param param;

	// �o�b�t�@
	std::map<std::string, Info>info;
};