#pragma once
#include <map>
#include <memory>

enum D3D12_PRIMITIVE_TOPOLOGY_TYPE : int;
struct D3D12_INPUT_ELEMENT_DESC;
class Device;
class Swap;
class Root;
class Pipe;

class PipeMane
{
public:
	// �R���X�g���N�^
	PipeMane(std::weak_ptr<Device>dev, std::weak_ptr<Swap>swap);
	// �f�X�g���N�^
	~PipeMane();

	// �p�C�v���C���̐���
	void CreatePipe(int& i, std::weak_ptr<Root>root, const D3D12_PRIMITIVE_TOPOLOGY_TYPE& type, const std::initializer_list<int>&index);

	// �p�C�v���C���N���X�̎擾
	std::shared_ptr<Pipe>Get(int& i) {
		return pipe[&i];
	}

private:
	// �f�o�C�X
	std::weak_ptr<Device>dev;

	// �X���b�v�`�F�C��
	std::weak_ptr<Swap>swap;

	// �p�C�v���C��
	std::map<int*, std::shared_ptr<Pipe>>pipe;
};
