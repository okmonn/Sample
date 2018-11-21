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
	List(std::weak_ptr<Device>dev, const D3D12_COMMAND_LIST_TYPE& type = (D3D12_COMMAND_LIST_TYPE)0);
	// デストラクタ
	~List();

	// アロケータの取得
	ID3D12CommandAllocator* GetAllo(void) const {
		return allo;
	}
	// リストの取得
	ID3D12GraphicsCommandList* GetList(void) const {
		return list;
	}

private:
	// アロケータの生成
	long CreateAllo(const D3D12_COMMAND_LIST_TYPE& type);

	// リストの生成
	long CreateList(const D3D12_COMMAND_LIST_TYPE& type);


	// デバイス
	std::weak_ptr<Device>dev;

	// アロケータ
	ID3D12CommandAllocator* allo;

	// リスト
	ID3D12GraphicsCommandList* list;
};
