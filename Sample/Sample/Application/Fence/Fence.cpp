#include "Fence.h"
#include "../Device/Device.h"
#include "../Queue/Queue.h"
#include "../Release.h"

// コンストラクタ
Fence::Fence(std::weak_ptr<Device> dev, std::weak_ptr<Queue>queue) :
	dev(dev), queue(queue), fence(nullptr), handle(CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS)), cnt(0)
{
	Create();
}

// デストラクタ
Fence::~Fence()
{
	Release(fence);
	CloseHandle(handle);
}

// フェンスの生成
long Fence::Create(void)
{
	auto hr = dev.lock()->GetDev()->CreateFence(cnt, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nフェンスの生成：失敗\n"));
		return hr;
	}

	//フェンス値の更新
	cnt = 1;

	return hr;
}

// 待機
void Fence::Wait(void)
{
	//フェンス値更新
	++cnt;

	//フェンス値を変更
	auto hr = queue.lock()->Get()->Signal(fence, cnt);
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nフェンス値の更新：失敗\n"));
		return;
	}

	//完了を待機(ポーリング)
	while (fence->GetCompletedValue() != cnt)
	{
		//フェンスイベントのセット
		hr = fence->SetEventOnCompletion(cnt, handle);
		if (FAILED(hr))
		{
			OutputDebugString(_T("\nフェンスイベントのセット：失敗\n"));
			return;
		}

		//フェンスイベントの待機
		WaitForSingleObject(handle, INFINITE);
	}
}
