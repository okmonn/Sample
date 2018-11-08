#pragma once
#include <vector>
#include <memory>

struct ID3D12GraphicsCommandList;
struct ID3D12DescriptorHeap;
struct ID3D12Resource;
class Device;
class Swap;

class Render
{
public:
	// コンストラクタ
	Render(std::weak_ptr<Device>dev, std::weak_ptr<Swap>swap);
	// デストラクタ
	~Render();

	// レンダーターゲットのセット
	void SetRender(ID3D12GraphicsCommandList* list, ID3D12DescriptorHeap* depth, const float& color);

	// ヒープの取得
	ID3D12DescriptorHeap* GetHeap(void) const {
		return heap;
	}
	// リソースの取得
	ID3D12Resource* GetRsc(const unsigned int& i) const {
		return rsc[i];
	}

private:
	// ヒープの生成
	long CreateHeap(void);

	// リソースとレンダーターゲットビューの生成
	long CreateRscView(void);

	// 初期化
	void Init(void);


	// デバイス
	std::weak_ptr<Device>dev;

	// スワップチェイン
	std::weak_ptr<Swap>swap;

	// ヒープ
	ID3D12DescriptorHeap* heap;

	// リソース
	std::vector<ID3D12Resource*>rsc;
};
