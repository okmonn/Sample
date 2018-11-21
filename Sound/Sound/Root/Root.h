#pragma once
#include <string>
#include <memory>

struct ID3D10Blob;
typedef ID3D10Blob ID3Blob;
struct ID3D12RootSignature;
class Device;

class Root
{
public:
	// コンストラクタ
	Root(std::weak_ptr<Device>dev, const std::wstring& fileName);
	// デストラクタ
	~Root();

	// ルートシグネチャの取得
	ID3D12RootSignature* Get(void) const {
		return root;
	}
	// シェーダー情報の取得
	ID3DBlob* GetShader(void) const {
		return shader;
	}

private:
	// シェーダーコンパイル
	long Compile(const std::wstring& fileName);

	// ルートシグネチャの情報を取得
	long GetRootInfo(void);

	// ルートシグネチャの生成
	long CreateRoot(void);


	// デバイス
	std::weak_ptr<Device>dev;

	// ルートシグネチャ
	ID3D12RootSignature* root;

	// メッセージ
	ID3DBlob* sig;

	// シェーダー情報
	ID3Blob* shader;
};
