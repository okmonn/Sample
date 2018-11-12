#include "PipeCompute.h"
#include "../Device/Device.h"
#include "../Root/RootCompute.h"
#include "../Release.h"

// �R���X�g���N�^
PipeCompute::PipeCompute(std::weak_ptr<Device>dev, std::weak_ptr<RootCompute>root) :
	dev(dev), root(root)
{
	Create();
}

// �f�X�g���N�^
PipeCompute::~PipeCompute()
{
	Release(pipe);
}

// �p�C�v���C���̐���
long PipeCompute::Create(void)
{
	D3D12_COMPUTE_PIPELINE_STATE_DESC desc{};
	desc.CS.pShaderBytecode = root.lock()->GetCompute()->GetBufferPointer();
	desc.CS.BytecodeLength  = root.lock()->GetCompute()->GetBufferSize();
	desc.Flags              = D3D12_PIPELINE_STATE_FLAGS::D3D12_PIPELINE_STATE_FLAG_NONE;
	desc.NodeMask           = 0;
	desc.pRootSignature     = root.lock()->Get();

	auto hr = dev.lock()->GetDev()->CreateComputePipelineState(&desc, IID_PPV_ARGS(&pipe));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n�R���s���[�g�p�C�v���C���̐����F���s\n"));
	}

	return 0;
}
