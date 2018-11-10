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
#include <d3d12.h>
#include <dxgi1_6.h>

// クリア色
const float color[] = {
	1.0f,
	1.0f,
	1.0f,
	1.0f
};
int n = 0;
int m = 0;
// コンストラクタ
Union::Union()
{
	rootNo.clear();
	pipeNo.clear();

	dev  = std::make_shared<Device>();
	com  = std::make_unique<Command>(dev);
	swap = std::make_shared<Swap>(dev, com->GetQueue());
	ren  = std::make_unique<Render>(dev, swap);
	dep  = std::make_unique<Depth>(dev);
	fen  = std::make_unique<Fence>(dev, com->GetQueue());
	root = std::make_unique<RootMane>(dev);
	pipe = std::make_unique<PipeMane>(dev, swap);

	CreateRoot();
	CreatePipe();

	tex = std::make_unique<Texture>(dev, root->Get(rootNo["sample"]), pipe->Get(pipeNo["sample"]));
	tex->Load("avicii.png", n);
	tex->Load("sample.bmp", m);
}

// デストラクタ
Union::~Union()
{
}

// 描画準備
void Union::Set(void)
{
	com->GetList()->Reset(nullptr);
	com->GetList()->SetViewport();
	com->GetList()->SetScissor();

	dep->SetDepth(com->GetList()->GetList());

	com->GetList()->SetBarrier(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET,
		ren->GetRsc(swap->Get()->GetCurrentBackBufferIndex()));

	ren->SetRender(com->GetList()->GetList(), dep->GetHeap(), *color);

	tex->Draw(com->GetList(), m, { 0.0f, 0.0f }, {100.0f, 100.0f}, { 0.0f, 0.0f }, { 256.0f, 256.0f });
	tex->Draw(com->GetList(), n, { 0.0f, 0.0f }, {640.0f, 480.0f},  { 0.0f, 0.0f }, { 200.0f, 200.0f });
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
	root->CreateRoot(rootNo[name], fileName);
}

// ルートシグネチャの生成
void Union::CreateRoot(void)
{
	CreateRoot("sample", L"Shader/Texture.hlsl");
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
	pipe->CreatePipe(pipeNo[name], root->Get(rootNo[rootName]), type, index, depth);
}

// パイプラインの生成
void Union::CreatePipe(void)
{
	CreatePipe("sample", "sample", D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, { 0, 2 }, false);
}

