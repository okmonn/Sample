#pragma once
#include <memory>

class Window;

class Application
{
public:
	// コンストラクタ
	Application(const unsigned int& width = 640, const unsigned int& height = 480);
	// デストラクタ
	~Application();

	// メッセージの確認
	bool CheckMsg(void);

	// ウィンドウのコールバック
	static long __stdcall WindowProc(void* hWnd, unsigned int message, long wParam, long lParam);

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
	std::shared_ptr<Window>win;

	// ウィンドウハンドル
	static void* winHandle;

	// ウィンドウ横幅
	static unsigned int width;

	// ウィンドウ縦幅
	static unsigned int height;
};
