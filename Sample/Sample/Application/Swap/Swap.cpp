#include "Swap.h"
#include "../Application.h"
#include "../Device/Device.h"
#include "../Queue/Queue.h"
#include "../Release.h"
#include <dxgi1_6.h>

// コンストラクタ
Swap::Swap(std::weak_ptr<Device>dev, std::weak_ptr<Queue>queue) :
	dev(dev), queue(queue), swap(nullptr)
{
	Create();
}

// デストラクタ
Swap::~Swap()
{
	Release(swap);
}

// スワップチェインの生成
long Swap::Create(void)
{
	DXGI_SWAP_CHAIN_DESC1 desc{};
	desc.AlphaMode   = DXGI_ALPHA_MODE::DXGI_ALPHA_MODE_UNSPECIFIED;
	desc.BufferCount = 2;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.Flags       = 0;
	desc.Format      = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.Height      = Application::GetHeight();
	desc.SampleDesc  = { 1, 0 };
	desc.Scaling     = DXGI_SCALING::DXGI_SCALING_STRETCH;
	desc.Stereo      = false;
	desc.SwapEffect  = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;
	desc.Width       = Application::GetWidth();

	auto hr = dev.lock()->GetFactory()->CreateSwapChainForHwnd(queue.lock()->Get(), reinterpret_cast<HWND>(Application::GetWinHandle()), &desc, nullptr, nullptr, reinterpret_cast<IDXGISwapChain1**>(&swap));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nスワップチェインの生成：失敗\n"));
	}

	return hr;
}

// 裏表を反転
void Swap::Present(void)
{
	swap->Present(1, 0);
}
