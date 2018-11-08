#pragma once
#include <memory>

enum D3D12_COMMAND_LIST_TYPE : int;
class Device;
class List;
class Queue;

class Command
{
public:
	// コンストラクタ
	Command(std::weak_ptr<Device>dev, const D3D12_COMMAND_LIST_TYPE& type = static_cast<D3D12_COMMAND_LIST_TYPE>(0));
	// デストラクタ
	~Command();

	// コマンドキュークラスの取得
	std::shared_ptr<Queue>GetQueue(void) const {
		return queue;
	}
	// コマンドリストクラスの取得
	std::shared_ptr<List>GetList(void) const {
		return list;
	}

private:
	// コマンドキュー
	std::shared_ptr<Queue>queue;

	// コマンドリスト
	std::shared_ptr<List>list;
};
