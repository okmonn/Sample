#pragma once
#include "../../etc/tString.h"
#include <map>
#include <memory>

enum D3D12_PRIMITIVE_TOPOLOGY_TYPE : int;
class Device;
class Command;
class Swap;
class Render;
class Depth;
class Fence;
class RootMane;
class PipeMane;
class DescriptorMane;

class Union
{
public:
	// コンストラクタ
	Union();
	// デストラクタ
	~Union();

	// 描画準備
	void Set(void);

	// 描画実行
	void Do(void);

private:
	// ルートシグネチャの生成
	void CreateRoot(const std::string& name, const std::tstring& fileName);
	void CreateRoot(void);

	// パイプラインの生成
	void CreatePipe(const std::string& name, const std::string& rootName, const D3D12_PRIMITIVE_TOPOLOGY_TYPE& type, const std::initializer_list<int>&index);
	void CreatePipe(void);


	// デバイス
	std::shared_ptr<Device>dev;

	// コマンド
	std::unique_ptr<Command>com;

	// スワップチェイン
	std::shared_ptr<Swap>swap;

	// レンダー
	std::unique_ptr<Render>ren;

	// デプス
	std::unique_ptr<Depth>dep;

	// フェンス
	std::unique_ptr<Fence>fen;

	// ルートマネジャー
	std::unique_ptr<RootMane>root;

	// パイプマネジャー
	std::unique_ptr<PipeMane>pipe;

	// ヒープ・リソースマネージャー
	std::unique_ptr<DescriptorMane>descriptorMane;

	// ルートシグネチャID
	std::map<std::string, int>rootNo;

	// パイプラインID
	std::map<std::string, int>pipeNo;
};
