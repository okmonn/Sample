#pragma once
#include <map>
#include <string>
#include <memory>
#include <DirectXMath.h>

struct ID3D12GraphicsCommandList;
struct D3D12_SUBRESOURCE_DATA;
struct ID3D12Resource;
class DescriptorMane;
class TextureLoad;
class Device;
class List;
class Root;
class Pipe;

namespace tex {
	// 頂点
	struct Vertex {
		//座標
		DirectX::XMFLOAT3 pos;
		//uv
		DirectX::XMFLOAT2 uv;
	};
}

class Texture
{
	//画像データ
	struct Tex {
		//画像リソース
		ID3D12Resource* rsc;
		//デコード
		unsigned char* decode;
		//サブデータ
		std::weak_ptr<D3D12_SUBRESOURCE_DATA>sub;
		//頂点リソースID
		int vRsc;
		//頂点送信データ
		unsigned int* data;
		//バンドルリスト
		std::unique_ptr<List>list;
	};

public:
	// コンストラクタ
	Texture(std::weak_ptr<Device>dev, std::weak_ptr<Root>root, std::weak_ptr<Pipe>pipe);
	// デストラクタ
	~Texture();

	// 読み込み
	void Load(const std::string& fileName, int& i);

	// 描画
	void Draw(ID3D12GraphicsCommandList* list, int& i);

	// 削除
	void Delete(int& i);

private:
	// シェーダーリソースビューの生成
	void CreateShaderView(int* i);

	// サブリソースに書き込み
	long WriteSub(int* i);

	// 頂点リソースの生成
	long CreateVertexRsc(int* i);

	// 頂点マップ
	long Map(int* i);

	// バンドルのセット
	void SetBundle(int* i);


	// ディスクリプターマネージャー
	DescriptorMane& descMane;

	// テクスチャローダー
	TextureLoad& loader;

	// デバイス
	std::weak_ptr<Device>dev;

	// ルートシグネチャ
	std::weak_ptr<Root>root;

	// パイプライン
	std::weak_ptr<Pipe>pipe;

	// 画像データ
	std::map<int*, Tex>tex;
};
