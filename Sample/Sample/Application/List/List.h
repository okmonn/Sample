#pragma once
#include <memory>

enum D3D12_COMMAND_LIST_TYPE : int;
struct ID3D12CommandAllocator;
struct ID3D12GraphicsCommandList;
class Device;

class List
{
public:
	// コンストラクタ
	List(std::weak_ptr<Device>dev, const D3D12_COMMAND_LIST_TYPE& type);
	// デストラクタ
	~List();

	// コマンドアロケータの取得
	ID3D12CommandAllocator* GetAllo(void) const {
		return allo;
	}
	// コマンドリストの取得
	ID3D12GraphicsCommandList* GetList(void) const {
		return list;
	}

private:
	// コマンドアロケータの生成
	long CreateAllo(const D3D12_COMMAND_LIST_TYPE& type);

	// コマンドリストの生成
	long CreateList(const D3D12_COMMAND_LIST_TYPE& type);

	// 初期化
	void Init(const D3D12_COMMAND_LIST_TYPE & type);


	// デバイス
	std::weak_ptr<Device>dev;

	// コマンドアロケータ
	ID3D12CommandAllocator* allo;

	// コマンドリスト
	ID3D12GraphicsCommandList* list;
};
