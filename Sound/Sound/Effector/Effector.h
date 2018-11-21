#pragma once
#include <map>
#include <string>
#include <vector>
#include <memory>

struct ID3D12DescriptorHeap;
struct ID3D12Resource;
class Queue;
class List;
class Device;
class Root;
class Pipe;

class Effector
{
	struct Info {
		//リソース
		ID3D12Resource* rsc;
		//送信データ
		float* data;
		//番号
		int index;
	};

	// パラメータ
	struct Param {
		//減衰率
		float attenuation;
		//遅延時間
		float time;
		//ループ回数
		int loop;
		//サンプリング周波数
		int sample;
	};

public:
	// コンストラクタ
	Effector(std::weak_ptr<Device>dev, std::weak_ptr<Root>root, std::weak_ptr<Pipe>pipe);
	// デストラクタ
	~Effector();

	// 実行
	void Execution(const std::vector<float>& wave, std::vector<float>& adaptation);

private:
	// ヒープの生成
	long CreateHeap(void);

	// CBVリソースの生成
	long CreateCbvRsc(const std::string& name, const unsigned int& size);

	// UAVリソースの生成
	long CreateUavRsc(const std::string& name, const unsigned int& size);

	// CBVの生成
	void CreateConstantView(const std::string& name, const unsigned int& size);

	// UAVの生成
	void CreateUnorderView(const std::string& name, const unsigned int& stride, const unsigned int& num);

	// マップ
	long Map(const std::string& name);

	// CBVの生成
	void CBV(const std::string& name, const unsigned int& size);

	// UAVの生成
	void UAV(const std::string& name, const unsigned int& stride, const unsigned int& num);

	// 初期化
	void Init(void);


	// デバイス
	std::weak_ptr<Device>dev;

	// キュー
	std::unique_ptr<Queue>queue;

	// リスト
	std::unique_ptr<List>list;

	// ルート
	std::weak_ptr<Root>root;

	// パイプ
	std::weak_ptr<Pipe>pipe;

	// ヒープ
	ID3D12DescriptorHeap* heap;

	// リソース番号
	int index;

	// パラメータ
	Param param;

	// バッファ
	std::map<std::string, Info>info;
};
