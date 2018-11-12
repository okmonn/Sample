#pragma once
#include "../../etc/tString.h"
#include <memory>

struct ID3D10Blob;
typedef ID3D10Blob ID3DBlob;
struct ID3D12RootSignature;
class Device;

class RootCompute
{
public:
	// コンストラクタ
	RootCompute(std::weak_ptr<Device>dev, const std::tstring& fileName);
	// デストラクタ
	~RootCompute();

	// ルートシグネチャの取得
	ID3D12RootSignature* Get(void) const {
		return root;
	}
	// メッセージの取得
	ID3DBlob* GetSig(void) const {
		return sig;
	}
	// エラーメッセージの取得
	ID3DBlob* GetError(void) const {
		return error;
	}
	// 頂点シェーダの取得
	ID3DBlob* GetCompute(void) const {
		return compute;
	}

private:
	// シェーダ読み込み
	long ShaderCompile(const std::tstring & fileName);

	// ルートシグネチャの生成
	long Create(void);


	// デバイス
	std::weak_ptr<Device>dev;

	// ルートシグネチャ
	ID3D12RootSignature* root;

	// メッセージ
	ID3DBlob* sig;

	// エラーメッセージ
	ID3DBlob* error;

	// 頂点シェーダ
	ID3DBlob* compute;
};
