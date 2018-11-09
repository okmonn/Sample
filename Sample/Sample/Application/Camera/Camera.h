#pragma once
#include <DirectXMath.h>

class DescriptorMane;

class Camera
{
	// �r���[�v���W�F�N�V����
	struct VP {
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 projection;
	};

public:
	// �R���X�g���N�^
	Camera();
	// �f�X�g���N�^
	~Camera();

private:
	// �f�B�X�N���v�^�}�l�[�W���[
	DescriptorMane& descMane;

	// �q�[�v�ƃ��\�[�X��ID
	int id;

	// ���M�f�[�^
	VP* data;
};
