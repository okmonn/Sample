#include "Compute.h"
#include "../DescriptorMane/DescriptorMane.h"
#include "../Device/Device.h"
#include "../Release.h"

// �R���X�g���N�^
Compute::Compute(std::weak_ptr<Device>dev) :
	descMane(DescriptorMane::Get()), dev(dev)
{
}

// �f�X�g���N�^
Compute::~Compute()
{
}
