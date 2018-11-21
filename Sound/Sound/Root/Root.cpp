#include <d3dcompiler.h>
#include "Root.h"
#include "../Device/Device.h"
#include "../Release.h"

#pragma comment(lib, "d3dcompiler.lib")

// コンストラクタ
Root::Root(std::weak_ptr<Device> dev, const std::wstring& fileName) : 
	dev(dev), root(nullptr), sig(nullptr), shader(nullptr)
{
	Compile(fileName);
	GetRootInfo();
	CreateRoot();
}

// デストラクタ
Root::~Root()
{
	Release(shader);
	Release(sig);
	Release(root);
}

// シェーダーコンパイル
long Root::Compile(const std::wstring & fileName)
{
	auto hr = D3DCompileFromFile(fileName.c_str(), nullptr, nullptr, "CS", "cs_5_1", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &shader, nullptr);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nシェーダの読み込み：失敗\n"));
	}

	return hr;
}

// ルートシグネチャの情報を取得
long Root::GetRootInfo(void)
{
	auto hr = D3DGetBlobPart(shader->GetBufferPointer(), shader->GetBufferSize(), D3D_BLOB_ROOT_SIGNATURE, 0, &sig);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nルートシグネチャ情報の取得：失敗\n"));
	}

	return hr;
}

// ルートシグネチャの生成
long Root::CreateRoot(void)
{
	auto hr = dev.lock()->Get()->CreateRootSignature(0, sig->GetBufferPointer(), sig->GetBufferSize(), IID_PPV_ARGS(&root));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nルートシグネチャの生成：失敗\n"));
	}

	return hr;
}
