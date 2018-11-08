#pragma once
#include <memory>

struct ID3D12GraphicsCommandList;
struct ID3D12DescriptorHeap;
struct ID3D12Resource;
class Device;

class Depth
{
public:
	// コンストラクタ
	Depth(std::weak_ptr<Device>dev);
	// デストラクタ
	~Depth();

	// デプスのセット・クリア
	void SetDepth(ID3D12GraphicsCommandList* list);

	// ヒープの取得
	ID3D12DescriptorHeap* GetHeap(void) const {
		return heap;
	}
	// リソースの取得
	ID3D12Resource* GetRsc(void) const {
		return rsc;
	}

private:
	// ヒープの生成
	long CreateHeap(void);

	// リソースの生成
	long CreateRsc(void);

	// ビューの生成
	void CreateView(void);

	// 初期化
	void Init(void);


	// デバイス
	std::weak_ptr<Device>dev;

	// ヒープ
	ID3D12DescriptorHeap* heap;

	// リソース
	ID3D12Resource* rsc;
};
