#pragma once
#include <memory>

class DescriptorMane;
class Device;
class Command;

class Compute
{
public:
	// コンストラクタ
	Compute(std::weak_ptr<Device>dev);
	// デストラクタ
	~Compute();

private:
	// ヒープ・リソースマネジャー
	DescriptorMane& descMane;

	// デバイス
	std::weak_ptr<Device>dev;
};
