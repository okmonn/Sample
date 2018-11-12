#pragma once
#include <memory>

struct ID3D12PipelineState;
class Device;
class RootCompute;

class PipeCompute
{
public:
	// �R���X�g���N�^
	PipeCompute(std::weak_ptr<Device>dev, std::weak_ptr<RootCompute>root);
	// �f�X�g���N�^
	~PipeCompute();

	// �p�C�v���C���̎擾
	ID3D12PipelineState* Get(void) const {
		return pipe;
	}

private:
	// �p�C�v���C���̐���
	long Create(void);


	// �f�o�C�X
	std::weak_ptr<Device>dev;

	// ���[�g�V�O�l�`��
	std::weak_ptr<RootCompute>root;

	// �p�C�v���C��
	ID3D12PipelineState* pipe;
};
