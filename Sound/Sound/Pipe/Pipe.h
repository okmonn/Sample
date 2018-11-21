#pragma once
#include <memory>

struct ID3D12PipelineState;
class Device;
class Root;

class Pipe
{
public:
	// コンストラクタ
	Pipe(std::weak_ptr<Device>dev, std::weak_ptr<Root>root);
	// デストラクタ
	~Pipe();

	// パイプラインの取得
	ID3D12PipelineState* Get(void) const {
		return pipe;
	}

private:
	// パイプラインの生成
	long CreatePipe(void);


	// デバイス
	std::weak_ptr<Device>dev;

	// ルートシグネチャ
	std::weak_ptr<Root>root;

	// パイプライン
	ID3D12PipelineState* pipe;
};
