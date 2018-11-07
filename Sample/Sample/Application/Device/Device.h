#pragma once

struct ID3D12Device;
struct IDXGIFactory4;
struct IDXGIAdapter1;

class Device
{
public:
	// コンストラクタ
	Device();
	// デストラクタ
	~Device();

	// デバイスの取得
	ID3D12Device* GetDev(void) const {
		return dev;
	}
	// ファクトリーの取得
	IDXGIFactory4* GetFactory(void) const {
		return factory;
	}
	// アダプターの取得
	IDXGIAdapter1* GetAdapter(void) const {
		return adapter;
	}
 
private:
	// ファクトリーの生成
	long CreateFactory(void);

	// デバイスの生成
	long CreateDev(void);

	// 初期化
	void Init(void);


	// デバイス
	ID3D12Device* dev;

	// ファクトリー
	IDXGIFactory4* factory;

	// アダプター
	IDXGIAdapter1* adapter;
};
