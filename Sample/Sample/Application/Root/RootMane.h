#pragma once
#include "../../etc/tString.h"
#include <map>
#include <memory>

class Device;
class Root;
class RootCompute;

class RootMane
{
public:
	// �f�X�g���N�^
	~RootMane();

	// �C���X�^���X�ϐ��̎擾
	static RootMane& Get(void) {
		static RootMane instance;
		return instance;
	}

	// ���[�g�V�O�l�`���N���X�̐���
	void CreateRoot(int& i, std::weak_ptr<Device>dev, const std::tstring& fileName);

	// ���[�g�V�O�l�`���R���s���[�g�N���X�̐���
	void CreateRootCompute(int& i, std::weak_ptr<Device>dev, const std::tstring& fileName);

	// ���[�g�V�O�l�`���N���X�̎擾
	std::shared_ptr<Root>Get(int& i) {
		return root[&i];
	}
	// ���[�g�V�O�l�`���R���s���[�g�N���X�̎擾
	std::shared_ptr<RootCompute>GetCompute(int& i) {
		return compute[&i];
	}

private:
	// �R���X�g���N�^
	RootMane();
	RootMane(const RootMane&) {
	}
	void operator=(const RootMane&) {
	}

	// ���[�g�V�O�l�`��
	std::map<int*, std::shared_ptr<Root>>root;

	// ���[�g�V�O�l�`���E�R���s���[�g
	std::map<int*, std::shared_ptr<RootCompute>>compute;
};