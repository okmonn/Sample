#pragma once
#include "../../etc/tString.h"
#include <map>
#include <memory>

class Device;
class Root;

class RootMane
{
public:
	// �R���X�g���N�^
	RootMane(std::weak_ptr<Device>dev);
	// �f�X�g���N�^
	~RootMane();

	// ���[�g�V�O�l�`���N���X�̐���
	void CreateRoot(int& i, const std::tstring& fileName);

	// ���[�g�V�O�l�`���N���X�̎擾
	std::shared_ptr<Root>Get(int& i) {
		return root[&i];
	}

private:
	// �f�o�C�X
	std::weak_ptr<Device>dev;

	// ���[�g�V�O�l�`��
	std::map<int*, std::shared_ptr<Root>>root;
};