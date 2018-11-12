#pragma once
#include "../../etc/tString.h"
#include <memory>

struct ID3D10Blob;
typedef ID3D10Blob ID3DBlob;
struct ID3D12RootSignature;
class Device;

class RootCompute
{
public:
	// �R���X�g���N�^
	RootCompute(std::weak_ptr<Device>dev, const std::tstring& fileName);
	// �f�X�g���N�^
	~RootCompute();

	// ���[�g�V�O�l�`���̎擾
	ID3D12RootSignature* Get(void) const {
		return root;
	}
	// ���b�Z�[�W�̎擾
	ID3DBlob* GetSig(void) const {
		return sig;
	}
	// �G���[���b�Z�[�W�̎擾
	ID3DBlob* GetError(void) const {
		return error;
	}
	// ���_�V�F�[�_�̎擾
	ID3DBlob* GetCompute(void) const {
		return compute;
	}

private:
	// �V�F�[�_�ǂݍ���
	long ShaderCompile(const std::tstring & fileName);

	// ���[�g�V�O�l�`���̐���
	long Create(void);


	// �f�o�C�X
	std::weak_ptr<Device>dev;

	// ���[�g�V�O�l�`��
	ID3D12RootSignature* root;

	// ���b�Z�[�W
	ID3DBlob* sig;

	// �G���[���b�Z�[�W
	ID3DBlob* error;

	// ���_�V�F�[�_
	ID3DBlob* compute;
};
