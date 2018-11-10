#pragma once
#include <memory>
#include <DirectXMath.h>

class DescriptorMane;
class Device;
class List;

class Camera
{
	// ビュープロジェクション
	struct VP {
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 projection;
	};

public:
	// コンストラクタ
	Camera(std::weak_ptr<Device>dev);
	// デストラクタ
	~Camera();

private:
	// リソースの生成
	long CreateRsc(void);

	// 定数バッファビューの生成
	void CreateConView(void);

	// マップ
	long Map(void);

	// バンドルのセット
	void SetBundle(void);

	// 初期化
	void Init(void);


	// ディスクリプタマネージャー
	DescriptorMane& descMane;

	// デバイス
	std::weak_ptr<Device>dev;

	// バンドル
	std::unique_ptr<List>list;

	// ヒープとリソースのID
	int id;

	// 送信データ
	VP* data;
};
