#pragma once
#include <memory>
#include <DirectXMath.h>

class DescriptorMane;
class Device;
class List;

class Camera
{
	// �r���[�v���W�F�N�V����
	struct VP {
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 projection;
	};

public:
	// �R���X�g���N�^
	Camera(std::weak_ptr<Device>dev);
	// �f�X�g���N�^
	~Camera();

private:
	// ���\�[�X�̐���
	long CreateRsc(void);

	// �萔�o�b�t�@�r���[�̐���
	void CreateConView(void);

	// �}�b�v
	long Map(void);

	// �o���h���̃Z�b�g
	void SetBundle(void);

	// ������
	void Init(void);


	// �f�B�X�N���v�^�}�l�[�W���[
	DescriptorMane& descMane;

	// �f�o�C�X
	std::weak_ptr<Device>dev;

	// �o���h��
	std::unique_ptr<List>list;

	// �q�[�v�ƃ��\�[�X��ID
	int id;

	// ���M�f�[�^
	VP* data;
};
