#include <d3dcompiler.h>
#include "RootCompute.h"
#include "../Device/Device.h"
#include "../Release.h"

// �R���X�g���N�^
RootCompute::RootCompute(std::weak_ptr<Device>dev, const std::tstring & fileName) : 
	dev(dev), root(nullptr), sig(nullptr), error(nullptr), compute(nullptr)
{
	ShaderCompile(fileName);
	Create();
}

// �f�X�g���N�^
RootCompute::~RootCompute()
{
	Release(compute);
	Release(sig);
	Release(error);
	Release(root);
}

// �V�F�[�_�ǂݍ���
long RootCompute::ShaderCompile(const std::tstring & fileName)
{
	auto hr = D3DCompileFromFile(fileName.c_str(), nullptr, nullptr, "CS", "cs_5_1", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &compute, &error);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n�R���s���[�g�V�F�[�_�̓ǂݍ��݁F���s\n"));
		return hr;
	}

	//�V�F�[�_���烋�[�g�V�O�l�`���̏����擾
	hr = D3DGetBlobPart(compute->GetBufferPointer(), compute->GetBufferSize(), D3D_BLOB_ROOT_SIGNATURE, 0, &sig);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n�R���s���[�g���[�g�V�O�l�`�����̎擾�F���s\n"));
	}

	return hr;
}

// ���[�g�V�O�l�`���̐���
long RootCompute::Create(void)
{
	auto hr = dev.lock()->GetDev()->CreateRootSignature(0, sig->GetBufferPointer(), sig->GetBufferSize(), IID_PPV_ARGS(&root));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n�R���s���[�g���[�g�V�O�l�`���̐����F���s\n"));
	}

	return hr;
}
