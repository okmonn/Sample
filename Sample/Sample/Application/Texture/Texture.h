#pragma once
#include <memory>

class Device;

class Texture
{
public:
	// コンストラクタ
	Texture(std::weak_ptr<Device>dev);
	// デストラクタ
	~Texture();

private:
	// デバイス
	std::weak_ptr<Device>dev;
};
