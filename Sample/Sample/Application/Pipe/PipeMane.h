#pragma once
#include <map>
#include <memory>

enum D3D12_PRIMITIVE_TOPOLOGY_TYPE : int;
struct D3D12_INPUT_ELEMENT_DESC;
class Device;
class Swap;
class Root;
class Pipe;

class PipeMane
{
public:
	// コンストラクタ
	PipeMane(std::weak_ptr<Device>dev, std::weak_ptr<Swap>swap);
	// デストラクタ
	~PipeMane();

	// パイプラインの生成
	void CreatePipe(int& i, std::weak_ptr<Root>root, const D3D12_PRIMITIVE_TOPOLOGY_TYPE& type, const std::initializer_list<int>&index);

	// パイプラインクラスの取得
	std::shared_ptr<Pipe>Get(int& i) {
		return pipe[&i];
	}

private:
	// デバイス
	std::weak_ptr<Device>dev;

	// スワップチェイン
	std::weak_ptr<Swap>swap;

	// パイプライン
	std::map<int*, std::shared_ptr<Pipe>>pipe;
};
