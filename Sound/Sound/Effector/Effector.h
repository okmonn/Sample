#pragma once
#include <string>
#include <memory>

struct ID3D12DescriptorHeap;
struct ID3D12Resource;
class Sound;
class Device;
class Root;
class Pipe;

class Effector
{
public:
	// コンストラクタ
	Effector(std::weak_ptr<Device>dev, std::weak_ptr<Root>root, std::weak_ptr<Pipe>pipe);
	// デストラクタ
	~Effector();

	// 読み込み
	void Load(const std::string& fileName);

private:
	// ヒープの生成
	long CreateHeap(void);

	// リソースの生成
	long CreateCbvRsc(const unsigned int& size);

	long CreateUavRsc(const unsigned int& size);


	// サウンド
	std::unique_ptr<Sound>sound;

	// デバイス
	std::weak_ptr<Device>dev;

	// ルート
	std::weak_ptr<Root>root;

	// パイプ
	std::weak_ptr<Pipe>pipe;

	// ヒープ
	ID3D12DescriptorHeap* heap;

	// 定数バッファ
	ID3D12Resource* cbv;

	// 非順序アクセスバッファ
	ID3D12Resource* uav;
};
