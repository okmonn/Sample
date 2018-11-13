#pragma once
#include <memory>

class DescriptorMane;
class Device;
class Command;
class Fence;
class RootCompute;
class PipeCompute;

class Compute
{
public:
	// コンストラクタ
	Compute(std::weak_ptr<Device>dev, std::weak_ptr<RootCompute>root, std::weak_ptr<PipeCompute>pipe);
	// デストラクタ
	~Compute();

	// 実行
	void Execution(void);

private:
	// UAV用リソースの生成
	long CreateUavRsc(void);

	// UAVビューの生成
	void CreateView(void);

	// コピーリソースの生成
	long CreateCopyRsc(void);

	// 初期化
	void Init(void);

	// マップ
	long Map(void);


	// ディスクリプタマネージャー
	DescriptorMane& descMane;

	// デバイス
	std::weak_ptr<Device>dev;

	// コマンド
	std::unique_ptr<Command>com;

	// フェンス
	std::unique_ptr<Fence>fence;

	// コンピュート用ルートシグネチャ
	std::weak_ptr<RootCompute>root;

	// コンピュート用パイプライン
	std::weak_ptr<PipeCompute>pipe;

	// UAVのID
	int uav;

	// コピーのID
	int copy;

	// 受信用データ
	float* data;
};
