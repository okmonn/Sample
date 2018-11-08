#pragma once
#include <memory>

class Device;

class Texture
{
public:
	// �R���X�g���N�^
	Texture(std::weak_ptr<Device>dev);
	// �f�X�g���N�^
	~Texture();

private:
	// �f�o�C�X
	std::weak_ptr<Device>dev;
};
