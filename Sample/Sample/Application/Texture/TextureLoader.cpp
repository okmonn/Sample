#include "TextureLoader.h"
#include "../Device/Device.h"
#include "../Release.h"
#include "WICTextureLoader12.h"
#include "../../etc/Func.h"

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
		Release(itr->second.rsc);
		itr->second.decode.release();
	}
}

// 読み込み
long TextureLoad::Load(std::weak_ptr<Device>dev, const std::string & fileName)
{
	if (origin.find(fileName) != origin.end())
	{
		return S_OK;
	}

	auto path = func::ChangeWString(fileName);
	origin[fileName].sub = std::make_shared<D3D12_SUBRESOURCE_DATA>();

	auto hr = DirectX::LoadWICTextureFromFile(dev.lock()->GetDev(), path.c_str(), &origin[fileName].rsc, origin[fileName].decode, (*origin[fileName].sub));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nテクスチャの読み込み：失敗\n"));
	}

	return hr;
}

// データの削除
void TextureLoad::Delete(const std::string & fileName)
{
	if (origin.find(fileName) != origin.end())
	{
		Release(origin[fileName].rsc);
		origin[fileName].decode.release();
		origin.erase(origin.find(fileName));
	}
}
