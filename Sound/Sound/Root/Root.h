#pragma once
#include <string>
#include <memory>

struct ID3D10Blob;
typedef ID3D10Blob ID3Blob;
struct ID3D12RootSignature;
class Device;

class Root
{
public:
	// �R���X�g���N�^
	Root(std::weak_ptr<Device>dev, const std::wstring& fileName);
	// �f�X�g���N�^
	~Root();

	// ���[�g�V�O�l�`���̎擾
	ID3D12RootSignature* Get(void) const {
		return root;
	}
	// �V�F�[�_�[���̎擾
	ID3DBlob* GetShader(void) const {
		return shader;
	}

private:
	// �V�F�[�_�[�R���p�C��
	long Compile(const std::wstring& fileName);

	// ���[�g�V�O�l�`���̏����擾
	long GetRootInfo(void);

	// ���[�g�V�O�l�`���̐���
	long CreateRoot(void);


	// �f�o�C�X
	std::weak_ptr<Device>dev;

	// ���[�g�V�O�l�`��
	ID3D12RootSignature* root;

	// ���b�Z�[�W
	ID3DBlob* sig;

	// �V�F�[�_�[���
	ID3Blob* shader;
};
