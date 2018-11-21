#include "Pipe.h"
#include "../Device/Device.h"
#include "../Root/Root.h"
#include "../Release.h"

// コンストラクタ
Pipe::Pipe(std::weak_ptr<Device> dev, std::weak_ptr<Root>root) :
	dev(dev), root(root), pipe(nullptr)
{
	CreatePipe();
}

// デストラクタ
Pipe::~Pipe()
{
	Release(pipe);
}

// パイプラインの生成
long Pipe::CreatePipe(void)
{
	D3D12_COMPUTE_PIPELINE_STATE_DESC desc{};
	desc.CS.pShaderBytecode = root.lock()->GetShader()->GetBufferPointer();
	desc.CS.BytecodeLength  = root.lock()->GetShader()->GetBufferSize();
	desc.Flags              = D3D12_PIPELINE_STATE_FLAGS::D3D12_PIPELINE_STATE_FLAG_NONE;
	desc.NodeMask           = 0;
	desc.pRootSignature     = root.lock()->Get();

	auto hr = dev.lock()->Get()->CreateComputePipelineState(&desc, IID_PPV_ARGS(&pipe));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nパイプラインの生成：失敗\n"));
	}

	return hr;
}
