#include "Pipe.h"
#include "../Device/Device.h"
#include "../Root/Root.h"
#include "../Release.h"

// �R���X�g���N�^
Pipe::Pipe(std::weak_ptr<Device> dev, std::weak_ptr<Root>root) :
	dev(dev), root(root), pipe(nullptr)
{
	CreatePipe();
}

// �f�X�g���N�^
Pipe::~Pipe()
{
	Release(pipe);
}

// �p�C�v���C���̐���
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
		OutputDebugString(_T("\n�p�C�v���C���̐����F���s\n"));
	}

	return hr;
}
