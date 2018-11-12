#include <d3dcompiler.h>
#include "RootCompute.h"
#include "../Device/Device.h"
#include "../Release.h"

// コンストラクタ
RootCompute::RootCompute(std::weak_ptr<Device>dev, const std::tstring & fileName) : 
	dev(dev), root(nullptr), sig(nullptr), error(nullptr), compute(nullptr)
{
	ShaderCompile(fileName);
	Create();
}

// デストラクタ
RootCompute::~RootCompute()
{
	Release(compute);
	Release(sig);
	Release(error);
	Release(root);
}

// シェーダ読み込み
long RootCompute::ShaderCompile(const std::tstring & fileName)
{
	auto hr = D3DCompileFromFile(fileName.c_str(), nullptr, nullptr, "CS", "cs_5_1", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &compute, &error);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nコンピュートシェーダの読み込み：失敗\n"));
		return hr;
	}

	//シェーダからルートシグネチャの情報を取得
	hr = D3DGetBlobPart(compute->GetBufferPointer(), compute->GetBufferSize(), D3D_BLOB_ROOT_SIGNATURE, 0, &sig);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nコンピュートルートシグネチャ情報の取得：失敗\n"));
	}

	return hr;
}

// ルートシグネチャの生成
long RootCompute::Create(void)
{
	auto hr = dev.lock()->GetDev()->CreateRootSignature(0, sig->GetBufferPointer(), sig->GetBufferSize(), IID_PPV_ARGS(&root));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nコンピュートルートシグネチャの生成：失敗\n"));
	}

	return hr;
}
