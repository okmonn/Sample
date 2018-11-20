#pragma once
#include <map>
#include <string>
#include <vector>
#include <memory>

class DescriptorMane;
class XAudio2;
class Sound;
class Device;
class List;
class Command;
class Fence;
class Root;
class RootCompute;
class Pipe;
class PipeCompute;

class DFT
{
	struct Data {
		int id;
		float* data;
	};

public:
	// コンストラクタ
	DFT(std::weak_ptr<Device>dev);
	// デストラクタ
	~DFT();

	// コンピュート用ルートシグネチャ・パイプラインのセット
	void SetCompute(std::weak_ptr<RootCompute>root, std::weak_ptr<PipeCompute>pipe);

	// グラフィックス用ルートシグネチャ・パイプラインのセット
	void SetGraphics(std::weak_ptr<Root>root, std::weak_ptr<Pipe>pipe);

	// 音声読み込み
	void Load(const std::string& fileName);

	// 実行
	void Execution(void);

	// 描画
	void Draw(std::weak_ptr<List>list);

private:
	// ヒープの生成
	long CreateHeap(void);

	// CBVリソースの生成
	long CreateCbvRsc(const std::string& name, const unsigned int& size);

	// UAVリソースの生成
	long CreateUavRsc(const std::string& name, const unsigned int& size);

	// CBVの生成
	void CreateCbv(const std::string& name, const unsigned int& size);

	// UAVの生成
	void CreateUav(const std::string& name, const unsigned int& stride, const unsigned int& num);

	// マップ
	long Map(const std::string& name);

	// CBVの生成
	void Cbv(const std::string& name, const unsigned int& size);

	// UAVの生成
	void Uav(const std::string& name, const unsigned int& stride, const unsigned int& num);

	// 頂点リソースの生成
	long CreateVertex(void);

	// 初期化
	void Init(void);


	// ディスクリプタマネージャー
	DescriptorMane& descMane;

	// XAudio2
	std::shared_ptr<XAudio2>audio;

	// サウンド
	std::unique_ptr<Sound>sound;

	// デバイス
	std::weak_ptr<Device>dev;

	// コマンド
	std::unique_ptr<Command>com;

	// フェンス
	std::unique_ptr<Fence>fen;

	// ルートシグネチャ・
	std::weak_ptr<Root>root;

	// コンピュートルートシグネチャ
	std::weak_ptr<RootCompute>cRoot;

	// パイプライン
	std::weak_ptr<Pipe>pipe;

	// コンピュートパイプライン
	std::weak_ptr<PipeCompute>cPipe;

	// リソース番号
	int nomber;

	// 波形配列番号
	unsigned int index;

	// ヒープID
	int heap;

	// リソースID
	std::map<std::string, Data>rsc;

	// 波形変換データ
	std::vector<float>wave;
};
