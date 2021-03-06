#pragma once
#include <map>
#include <string>
#include <memory>

struct D3D12_SUBRESOURCE_DATA;
struct ID3D12Resource;
class DescriptorMane;
class Device;

class TextureLoad
{
	// 元データ
	struct Origin {
		//リソース
		ID3D12Resource* rsc;
		//デコード
		std::unique_ptr<unsigned char[]>decode;
		//サブデータ
		std::shared_ptr<D3D12_SUBRESOURCE_DATA>sub;
	};

public:
	// デストラクタ
	~TextureLoad();

	// インスタンス変数の取得
	static TextureLoad& Get(void) {
		static TextureLoad instance;
		return instance;
	}

	// 読み込み
	long Load(std::weak_ptr<Device>dev, const std::string& fileName);

	// 画像の横幅の取得
	unsigned int GetWidth(const std::string& fileName);

	// 画像の横幅の取得
	unsigned int GetHeight(const std::string& fileName);

	// データの削除
	void Delete(const std::string& fileName);

	// リソースの取得
	ID3D12Resource* GetRsc(const std::string& fileName) {
		return origin[fileName].rsc;
	}
	// デコードの取得
	unsigned char* GetDecode(const std::string& fileName) {
		return origin[fileName].decode.get();
	}
	// サブデータの取得
	std::shared_ptr<D3D12_SUBRESOURCE_DATA>GetSub(const std::string& fileName) {
		return origin[fileName].sub;
	}

private:
	// コンストラクタ
	TextureLoad();
	TextureLoad(const TextureLoad&) {
	}
	void operator=(const TextureLoad&) {
	}

	// オリジンデータ
	std::map<std::string, Origin>origin;
};
