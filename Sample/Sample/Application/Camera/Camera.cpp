#include "Camera.h"
#include "../DescriptorMane/DescriptorMane.h"

// コンストラクタ
Camera::Camera() : 
	descMane(DescriptorMane::Get())
{
}

// デストラクタ
Camera::~Camera()
{
}
