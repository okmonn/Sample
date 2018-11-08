#include "RootMane.h"
#include "../Device/Device.h"
#include "Root.h"

// �R���X�g���N�^
RootMane::RootMane(std::weak_ptr<Device>dev) : 
	dev(dev)
{
	root.clear();
}

// �f�X�g���N�^
RootMane::~RootMane()
{
}

// ���[�g�V�O�l�`���N���X�̐���
void RootMane::CreateRoot(int& i, const std::tstring & fileName)
{
	root[&i] = std::make_shared<Root>(dev, fileName);
}
