#pragma once

struct IDirectInput8W;
struct IDirectInputDevice8W;

// キー最大数
#define KEY_MAX 256

class Input
{
public:
	// コンストラクタ
	Input();
	// デストラクタ
	~Input();

	// キー入力
	bool CheckKey(const int& i);

	// キートリガー
	bool Triger(const int& i);

private:
	// インプットの生成
	long CreateInput(void);

	// キーデバイスの生成
	long CreateDev(void);

	// キーフォーマットのセット
	long SetKeyFormat(void);

	// キーの協調レベルのセット
	long SetKeyCooperative(void);

	// 初期化
	void Init(void);


	// インプット
	IDirectInput8W* input;

	// インプットデバイス
	IDirectInputDevice8W* dev;

	// 現在のキー情報
	unsigned char keys[KEY_MAX];

	// 直前のキー情報
	unsigned char olds[KEY_MAX];
};
