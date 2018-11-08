#pragma once
#include "../../etc/tString.h"
#include <map>
#include <memory>

enum D3D12_PRIMITIVE_TOPOLOGY_TYPE : int;
class Device;
class Command;
class Swap;
class Render;
class Depth;
class Fence;
class RootMane;
class PipeMane;
class DescriptorMane;

class Union
{
public:
	// �R���X�g���N�^
	Union();
	// �f�X�g���N�^
	~Union();

	// �`�揀��
	void Set(void);

	// �`����s
	void Do(void);

private:
	// ���[�g�V�O�l�`���̐���
	void CreateRoot(const std::string& name, const std::tstring& fileName);
	void CreateRoot(void);

	// �p�C�v���C���̐���
	void CreatePipe(const std::string& name, const std::string& rootName, const D3D12_PRIMITIVE_TOPOLOGY_TYPE& type, const std::initializer_list<int>&index);
	void CreatePipe(void);


	// �f�o�C�X
	std::shared_ptr<Device>dev;

	// �R�}���h
	std::unique_ptr<Command>com;

	// �X���b�v�`�F�C��
	std::shared_ptr<Swap>swap;

	// �����_�[
	std::unique_ptr<Render>ren;

	// �f�v�X
	std::unique_ptr<Depth>dep;

	// �t�F���X
	std::unique_ptr<Fence>fen;

	// ���[�g�}�l�W���[
	std::unique_ptr<RootMane>root;

	// �p�C�v�}�l�W���[
	std::unique_ptr<PipeMane>pipe;

	// �q�[�v�E���\�[�X�}�l�[�W���[
	std::unique_ptr<DescriptorMane>descriptorMane;

	// ���[�g�V�O�l�`��ID
	std::map<std::string, int>rootNo;

	// �p�C�v���C��ID
	std::map<std::string, int>pipeNo;
};
