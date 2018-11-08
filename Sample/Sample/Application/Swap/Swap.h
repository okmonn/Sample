#pragma once
#include <memory>

struct IDXGISwapChain4;
class Device;
class Queue;

class Swap
{
public:
	// コンストラクタ
	Swap(std::weak_ptr<Device>dev, std::weak_ptr<Queue>queue);
	// デストラクタ
	~Swap();

	// 裏表を反転
	void Present(void);

	// スワップチェインの取得
	IDXGISwapChain4* Get(void) const {
		return swap;
	}

private:
	// スワップチェインの生成
	long Create(void);


	// デバイス
	std::weak_ptr<Device>dev;

	// コマンドキュー
	std::weak_ptr<Queue>queue;

	// スワップチェイン
	IDXGISwapChain4* swap;
};
