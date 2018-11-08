#include "Texture.h"

// コンストラクタ
Texture::Texture(std::weak_ptr<Device>dev) : 
	dev(dev)
{
}

// デストラクタ
Texture::~Texture()
{
}
