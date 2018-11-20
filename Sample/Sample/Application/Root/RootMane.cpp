#include "RootMane.h"
#include "../Device/Device.h"
#include "Root.h"
#include "RootCompute.h"

#pragma comment (lib, "d3dcompiler.lib")

// �R���X�g���N�^
RootMane::RootMane()
{
	root.clear();
	compute.clear();
}

// �f�X�g���N�^
RootMane::~RootMane()
{
}

// ���[�g�V�O�l�`���N���X�̐���
void RootMane::CreateRoot(int& i, std::weak_ptr<Device>dev, const std::tstring & fileName)
{
	root[&i] = std::make_shared<Root>(dev, fileName);
}

// ���[�g�V�O�l�`���R���s���[�g�N���X�̐���
void RootMane::CreateRootCompute(int & i, std::weak_ptr<Device>dev, const std::tstring & fileName)
{
	compute[&i] = std::make_shared<RootCompute>(dev, fileName);
}
