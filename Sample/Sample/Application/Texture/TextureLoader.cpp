#include "TextureLoader.h"
#include "../Release.h"

// �R���X�g���N�^
TextureLoad::TextureLoad()
{
	origin.clear();
}

// �f�X�g���N�^
TextureLoad::~TextureLoad()
{
	for (auto itr = origin.begin(); itr != origin.end(); ++itr)
	{
		itr->second.decode.release();
	}
}

// �ǂݍ���
long TextureLoad::Load(const std::string & fileName)
{
	return 0;
}
