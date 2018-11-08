#pragma once
#include<memory>

enum D3D12_COMMAND_LIST_TYPE : int;
struct ID3D12CommandQueue;
struct ID3D12CommandList;
class Device;

class Queue
{
public:
	// コンストラクタ
	Queue(std::weak_ptr<Device>dev, const D3D12_COMMAND_LIST_TYPE& type);
	// デストラクタ
	~Queue();

	// コマンドリストの実行
	void Execute(ID3D12CommandList** list, const unsigned int& num = 1);

	// コマンドキューの取得
	ID3D12CommandQueue* Get(void) const {
		return queue;
	}

private:
	// コマンドキューの生成
	long Create(const D3D12_COMMAND_LIST_TYPE& type);


	// デバイス
	std::weak_ptr<Device>dev;

	// コマンドキュー
	ID3D12CommandQueue* queue;
};
