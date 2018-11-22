#pragma once
#include <memory>

struct ID3D12Fence;
class Device;
class Queue;

class Fence
{
public:
	// コンストラクタ
	Fence(std::weak_ptr<Device>dev, std::weak_ptr<Queue>queue);
	// デストラクタ
	~Fence();

	// 待機
	void Wait(void);

	// フェンス取得
	ID3D12Fence* Get(void) const {
		return fence;
	}

private:
	// フェンスの生成
	long CreateFence(void);


	// デバイス
	std::weak_ptr<Device>dev;

	// キュー
	std::weak_ptr<Queue>queue;

	// フェンス
	ID3D12Fence* fence;

	// ハンドル
	void* handle;

	// フェンスカウント
	long long cnt;
};