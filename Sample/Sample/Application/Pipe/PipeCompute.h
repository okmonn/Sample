#pragma once
#include <memory>

struct ID3D12PipelineState;
class Device;
class RootCompute;

class PipeCompute
{
public:
	// コンストラクタ
	PipeCompute(std::weak_ptr<Device>dev, std::weak_ptr<RootCompute>root);
	// デストラクタ
	~PipeCompute();

	// パイプラインの取得
	ID3D12PipelineState* Get(void) const {
		return pipe;
	}

private:
	// パイプラインの生成
	long Create(void);


	// デバイス
	std::weak_ptr<Device>dev;

	// ルートシグネチャ
	std::weak_ptr<RootCompute>root;

	// パイプライン
	ID3D12PipelineState* pipe;
};
