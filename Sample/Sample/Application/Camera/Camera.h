#pragma once
#include <DirectXMath.h>

class DescriptorMane;

class Camera
{
	// ビュープロジェクション
	struct VP {
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 projection;
	};

public:
	// コンストラクタ
	Camera();
	// デストラクタ
	~Camera();

private:
	// ディスクリプタマネージャー
	DescriptorMane& descMane;

	// ヒープとリソースのID
	int id;

	// 送信データ
	VP* data;
};
