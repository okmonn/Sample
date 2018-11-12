#pragma once
#include <memory>

enum D3D12_COMMAND_LIST_TYPE : int;
enum D3D12_RESOURCE_STATES : int;
struct ID3D12CommandAllocator;
struct ID3D12GraphicsCommandList;
struct ID3D12Resource;
struct ID3D12RootSignature;
struct ID3D12PipelineState;
class Device;

class List
{
public:
	// コンストラクタ
	List(std::weak_ptr<Device>dev, const D3D12_COMMAND_LIST_TYPE& type);
	// デストラクタ
	~List();

	// コマンドリストを閉じる
	void Close(void);

	// リセット
	void Reset(ID3D12PipelineState* pipe);

	// ルートシグネチャのセット
	void SetRoot(ID3D12RootSignature* root);

	// パイプラインのセット
	void SetPipe(ID3D12PipelineState* pipe);

	// ビューポートのセット
	void SetViewport(void);

	// シザーのセット
	void SetScissor(void);

	// バリアのセット
	void SetBarrier(const D3D12_RESOURCE_STATES& befor, const D3D12_RESOURCE_STATES& affter, ID3D12Resource* rsc);

	// コンピュートバリアのセット
	void SetComputeBarrier(ID3D12Resource * rsc);

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
