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
class Texture;
class DFT;

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
	void CreateRootCompute(const std::string& name, const std::tstring& fileName);
	void CreateRoot(void);

	// パイプラインの生成
	void CreatePipe(const std::string& name, const std::string& rootName, const D3D12_PRIMITIVE_TOPOLOGY_TYPE& type, 
		const std::initializer_list<int>&index, const bool& depth = false);
	void CreatePipeCompute(const std::string& name, const std::string& rootName);
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
	RootMane& root;

	// パイプマネジャー
	PipeMane& pipe;

	// テクスチャ
	std::unique_ptr<Texture>tex;

	// DFT
	std::unique_ptr<DFT>dft;

	// ルートシグネチャID
	std::map<std::string, int>rootNo;

	// パイプラインID
	std::map<std::string, int>pipeNo;
};
