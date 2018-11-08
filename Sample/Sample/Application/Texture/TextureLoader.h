#pragma once
#include <map>
#include <string>
#include <memory>

struct D3D12_SUBRESOURCE_DATA;

class TextureLoad
{
	// 元データ
	struct Origin {
		//ヒープID
		int heap;
		//デコード
		std::unique_ptr<unsigned char[]>decode;
		//サブデータ
		std::shared_ptr<D3D12_SUBRESOURCE_DATA>sub;
	};
public:
	// コンストラクタ
	TextureLoad();
	// デストラクタ
	~TextureLoad();

	// 読み込み
	long Load(const std::string& fileName);

private:
	// オリジンデータ
	std::map<std::string, Origin>origin;
};
