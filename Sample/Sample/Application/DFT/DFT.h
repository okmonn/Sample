#pragma once
#include <map>
#include <string>
#include <vector>
#include <memory>

class SoundLoader;
class DescriptorMane;
class Device;
class Command;
class Fence;
class RootCompute;
class PipeCompute;

class DFT
{
	struct Info {
		int id;
		float* data;
	};
public:
	// コンストラクタ
	DFT(std::weak_ptr<Device>dev, std::weak_ptr<RootCompute>root, std::weak_ptr<PipeCompute>pipe);
	// デストラクタ
	~DFT();

	// 処理
	void UpData(void);

private:
	// ヒープの生成
	long CreateHeap(void);

	// CBVリソースの生成
	long CreateCbvRsc(const std::string& name);

	// CBVビューの生成
	void CreateCbvView(const std::string& name);

	// UAVリソースの生成
	long CreateUavRsc(const std::string& name);

	// UAVビューの生成
	void CreateUavView(const std::string& name);

	// リソースのマップ
	long Map(const std::string& name);

	// CBVの生成
	void CreateCbv(const std::string& name);

	// UAVの生成
	void CreateUav(const std::string& name);

	// 初期化
	void Init(void);


	// サウンドローダー
	SoundLoader& loader;

	// ヒープ・リソースマネージャー
	DescriptorMane& descMane;

	// デバイス
	std::weak_ptr<Device>dev;

	// コマンド
	std::unique_ptr<Command>com;

	// フェンス
	std::unique_ptr<Fence>fence;

	// コンピュートルートシグネチャ
	std::weak_ptr<RootCompute>root;

	// コンピュートパイプライン
	std::weak_ptr<PipeCompute>pipe;

	// ヒープID
	int heapID;

	// リソース番号
	int index;

	// リソースID
	std::map<std::string, Info>rsc;

	// 
	std::weak_ptr<std::vector<std::vector<float>>>data;
};
