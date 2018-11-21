#pragma once
#include <string>
#include <memory>

struct ID3D12DescriptorHeap;
struct ID3D12Resource;
class Sound;
class Device;
class Root;
class Pipe;

class Effector
{
public:
	// �R���X�g���N�^
	Effector(std::weak_ptr<Device>dev, std::weak_ptr<Root>root, std::weak_ptr<Pipe>pipe);
	// �f�X�g���N�^
	~Effector();

	// �ǂݍ���
	void Load(const std::string& fileName);

private:
	// �q�[�v�̐���
	long CreateHeap(void);

	// ���\�[�X�̐���
	long CreateCbvRsc(const unsigned int& size);

	long CreateUavRsc(const unsigned int& size);


	// �T�E���h
	std::unique_ptr<Sound>sound;

	// �f�o�C�X
	std::weak_ptr<Device>dev;

	// ���[�g
	std::weak_ptr<Root>root;

	// �p�C�v
	std::weak_ptr<Pipe>pipe;

	// �q�[�v
	ID3D12DescriptorHeap* heap;

	// �萔�o�b�t�@
	ID3D12Resource* cbv;

	// �񏇏��A�N�Z�X�o�b�t�@
	ID3D12Resource* uav;
};
