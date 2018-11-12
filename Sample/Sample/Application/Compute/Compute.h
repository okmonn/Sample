#pragma once
#include <memory>

class DescriptorMane;
class Device;
class List;
class Command;
class RootCompute;
class PipeCompute;

class Compute
{
public:
	// コンストラクタ
	Compute(std::weak_ptr<Device>dev, std::weak_ptr<RootCompute>root, std::weak_ptr<PipeCompute>pipe);
	// デストラクタ
	~Compute();

	// ルートシグネチャコンピュートのセット
	void SetRoot(void);

	// パイプラインコンピュートのセット
	void SetPipe(void);

	// 実行
	void Do(std::weak_ptr<List>list);

private:
	// リソースの生成
	long CreateRsc(void);

	// UAVビューの生成
	void CreateView(void);

	// マップ
	long Map(void);


	// ヒープ・リソースマネジャー
	DescriptorMane& descMane;

	// デバイス
	std::weak_ptr<Device>dev;

	// コマンド
	std::unique_ptr<Command>com;

	// ルートシグネチャ
	std::weak_ptr<RootCompute>root;

	// パイプライン
	std::weak_ptr<PipeCompute>pipe;

	// ヒープ・リソースID
	int id;

	int cId;

	// データ
	float* data;
};
