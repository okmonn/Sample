#pragma once
#include <map>
#include <memory>

enum D3D12_DESCRIPTOR_HEAP_FLAGS : int;
enum D3D12_DESCRIPTOR_HEAP_TYPE : int;
enum D3D12_RESOURCE_STATES:int;
struct D3D12_HEAP_PROPERTIES;
struct D3D12_RESOURCE_DESC;
struct ID3D12DescriptorHeap;
struct ID3D12Resource;
class Device;

class DescriptorMane
{
public:
	// コンストラクタ
	DescriptorMane(std::weak_ptr<Device>dev);
	// デストラクタ
	~DescriptorMane();

	// ヒープの生成
	long CreateHeap(int& i, const D3D12_DESCRIPTOR_HEAP_FLAGS& flag = static_cast<D3D12_DESCRIPTOR_HEAP_FLAGS>(0),
		const unsigned int& num = 1, const D3D12_DESCRIPTOR_HEAP_TYPE& type = static_cast<D3D12_DESCRIPTOR_HEAP_TYPE>(0));

	// リソースの生成
	long CreateRsc(int& i, const D3D12_HEAP_PROPERTIES& prop, const D3D12_RESOURCE_DESC& desc,
		const D3D12_RESOURCE_STATES& state = static_cast<D3D12_RESOURCE_STATES>(2755));

	// ヒープの取得
	std::shared_ptr<ID3D12DescriptorHeap*>GetHeap(int& i) {
		return heap[&i];
	}
	// リソースの取得
	std::shared_ptr<ID3D12Resource*>GetRsc(int& i) {
		return rsc[&i];
	}

private:
	// デバイス
	std::weak_ptr<Device>dev;

	// ヒープ
	std::map<int*, std::shared_ptr<ID3D12DescriptorHeap*>>heap;

	// リソース
	std::map<int*, std::shared_ptr<ID3D12Resource*>>rsc;
};
