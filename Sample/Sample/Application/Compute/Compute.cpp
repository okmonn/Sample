#include "Compute.h"
#include "../DescriptorMane/DescriptorMane.h"
#include "../Device/Device.h"
#include "../Release.h"

// コンストラクタ
Compute::Compute(std::weak_ptr<Device>dev) :
	descMane(DescriptorMane::Get()), dev(dev)
{
}

// デストラクタ
Compute::~Compute()
{
}
