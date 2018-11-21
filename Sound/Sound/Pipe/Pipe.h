#pragma once
#include <memory>

struct ID3D12PipelineState;
class Device;
class Root;

class Pipe
{
public:
	// �R���X�g���N�^
	Pipe(std::weak_ptr<Device>dev, std::weak_ptr<Root>root);
	// �f�X�g���N�^
	~Pipe();

	// �p�C�v���C���̎擾
	ID3D12PipelineState* Get(void) const {
		return pipe;
	}

private:
	// �p�C�v���C���̐���
	long CreatePipe(void);


	// �f�o�C�X
	std::weak_ptr<Device>dev;

	// ���[�g�V�O�l�`��
	std::weak_ptr<Root>root;

	// �p�C�v���C��
	ID3D12PipelineState* pipe;
};