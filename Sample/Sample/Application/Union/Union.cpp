#include "Union.h"
#include "../Device/Device.h"
#include "../Command/Command.h"
#include "../List/List.h"
#include "../Queue/Queue.h"
#include "../Swap/Swap.h"
#include "../Render/Render.h"
#include "../Depth/Depth.h"
#include "../Fence/Fence.h"
#include "../Root/RootMane.h"
#include "../Pipe/PipeMane.h"
#include "../Texture/Texture.h"
#include "../DFT/DFT.h"
#include <d3d12.h>
#include <dxgi1_6.h>

// クリア色
const float color[] = {
	0.0f,
	0.0f,
	0.0f,
	0.0f
};

// コンストラクタ
Union::Union() : 
	root(RootMane::Get()), pipe(PipeMane::Get())
{
	rootNo.clear();
	pipeNo.clear();

	dev  = std::make_shared<Device>();
	com  = std::make_unique<Command>(dev);
	swap = std::make_shared<Swap>(dev, com->GetQueue());
	ren  = std::make_unique<Render>(dev, swap);
	dep  = std::make_unique<Depth>(dev);
	fen  = std::make_unique<Fence>(dev, com->GetQueue());

	CreateRoot();
	CreatePipe();

	tex = std::make_unique<Texture>(dev, root.Get(rootNo["sample"]), pipe.Get(pipeNo["sample"]));

	dft = std::make_unique<DFT>(dev);
	dft->SetCompute(root.GetCompute(rootNo["compute"]), pipe.GetCompute(pipeNo["compute"]));
	dft->SetGraphics(root.Get(rootNo["draw"]), pipe.Get(pipeNo["draw"]));
	dft->Load("animal.wav");
}

// デストラクタ
Union::~Union()
{
}

// 描画準備
void Union::Set(void)
{
	dft->Execution();

	com->GetList()->Reset(nullptr);
	com->GetList()->SetViewport();
	com->GetList()->SetScissor();

	dep->SetDepth(com->GetList()->GetList());

	com->GetList()->SetBarrier(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET,
		ren->GetRsc(swap->Get()->GetCurrentBackBufferIndex()));

	ren->SetRender(com->GetList()->GetList(), dep->GetHeap(), *color);

	dft->Draw(com->GetList());
}

// 描画実行
void Union::Do(void)
{
	com->GetList()->SetBarrier(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT,
		ren->GetRsc(swap->Get()->GetCurrentBackBufferIndex()));

	com->GetList()->Close();

	ID3D12CommandList* list[] = {
		com->GetList()->GetList(),
	};
	com->GetQueue()->Execute(list, _countof(list));
	swap->Present();
	fen->Wait();
}

// ルートシグネチャの生成
void Union::CreateRoot(const std::string & name, const std::tstring & fileName)
{
	if (rootNo.find(name) != rootNo.end())
	{
		return;
	}

	rootNo[name] = 0;
	root.CreateRoot(rootNo[name], dev, fileName);
}

// ルートシグネチャの生成
void Union::CreateRootCompute(const std::string & name, const std::tstring & fileName)
{
	if (rootNo.find(name) != rootNo.end())
	{
		return;
	}

	rootNo[name] = 0;
	root.CreateRootCompute(rootNo[name], dev, fileName);
}

// ルートシグネチャの生成
void Union::CreateRoot(void)
{
	CreateRoot("sample", L"Shader/Texture.hlsl");
	CreateRootCompute("compute", L"Shader/DFT.hlsl");
	CreateRoot("draw", L"Shader/Draw.hlsl");
}

// パイプラインの生成
void Union::CreatePipe(const std::string & name, const std::string & rootName, const D3D12_PRIMITIVE_TOPOLOGY_TYPE & type, 
	const std::initializer_list<int> & index, const bool & depth)
{
	if (pipeNo.find(name) != pipeNo.end())
	{
		return;
	}

	pipeNo[name] = 0;
	pipe.CreatePipe(pipeNo[name], dev, swap, root.Get(rootNo[rootName]), type, index, depth);
}

// パイプラインの生成
void Union::CreatePipeCompute(const std::string & name, const std::string & rootName)
{
	if (pipeNo.find(name) != pipeNo.end())
	{
		return;
	}

	pipeNo[name] = 0;
	pipe.CreatePipeCompute(pipeNo[name], dev, root.GetCompute(rootNo[rootName]));
}

// パイプラインの生成
void Union::CreatePipe(void)
{
	CreatePipe("sample", "sample", D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, { 0, 2 }, false);
	CreatePipeCompute("compute", "compute");
	CreatePipe("draw", "draw", D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE, { 0 }, false);
}

