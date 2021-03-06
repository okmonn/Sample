#pragma once
#include <map>
#include <vector>
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

	struct Info
	{
		//移動行列
		DirectX::XMFLOAT4X4 matrix;
		//ウィンドウサイズ
		DirectX::XMFLOAT2 window;
		//uv座標
		DirectX::XMFLOAT2 uvPos;
		//uvサイズ
		DirectX::XMFLOAT2 uvSize;
		//反転
		DirectX::XMFLOAT2 reverse;
		//アルファ値
		float alpha;
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
		//定数リソースID
		int cRsc;
		//定数送信データ
		tex::Info* info;
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
	void Draw(std::weak_ptr<List>list, int& i, const DirectX::XMFLOAT2& pos, const DirectX::XMFLOAT2& size, 
		const DirectX::XMFLOAT2& uvPos, const DirectX::XMFLOAT2& uvSize, const float& alpha = 1.0f, 
		const bool& turnX = false, const bool& turnY = false);

	// 削除
	void Delete(int& i);

private:
	float a;
	// 頂点のセット
	void SetVertex(void);

	// 定数リソースの生成
	long CreateConRsc(int* i);

	// 定数バッファビューの生成
	void CreateConView(int* i);

	// シェーダーリソースビューの生成
	void CreateShaderView(int* i);

	// サブリソースに書き込み
	long WriteSub(int* i);

	// 定数バッファのマップ
	long MapCon(int* i);

	// 頂点リソースの生成
	long CreateVertexRsc(int* i);

	// 頂点マップ
	long MapVertex(int* i);

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

	// 頂点
	std::vector<tex::Vertex>vertex;

	// 画像データ
	std::map<int*, Tex>tex;
};
