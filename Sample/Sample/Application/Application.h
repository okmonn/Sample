#pragma once
#include "../Input/KeyData.h"
#include <memory>

class Window;
class Input;
class Union;

class Application
{
public:
	// コンストラクタ
	Application(const unsigned int& width = 640, const unsigned int& height = 480);
	// デストラクタ
	~Application();

	// メッセージの確認
	bool CheckMsg(void);

	// キーの入力
	bool CheckKey(const int& i);

	// 描画・処理
	void Render(void);

	// ウィンドウハンドルの取得
	static void* GetWinHandle(void) {
		return winHandle;
	}
	// ウィンドウ縦幅の取得
	static unsigned int GetWidth(void) {
		return width;
	}
	// ウィンドウ横幅
	static unsigned int GetHeight(void) {
		return height;
	}

private:
	// ウィンドウ
	std::unique_ptr<Window>win;

	// インプット
	std::unique_ptr<Input>input;

	// ユニオン
	std::unique_ptr<Union>un;

	// ウィンドウハンドル
	static void* winHandle;

	// ウィンドウ横幅
	static unsigned int width;

	// ウィンドウ縦幅
	static unsigned int height;
};
