#include "TextureLoader.h"
#include "../Release.h"

// コンストラクタ
TextureLoad::TextureLoad()
{
	origin.clear();
}

// デストラクタ
TextureLoad::~TextureLoad()
{
	for (auto itr = origin.begin(); itr != origin.end(); ++itr)
	{
		itr->second.decode.release();
	}
}

// 読み込み
long TextureLoad::Load(const std::string & fileName)
{
	return 0;
}
