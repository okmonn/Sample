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

	// ウィンドウ縦幅の取得
	unsigned int GetWidth(void) const {
		return width;
	}
	// ウィンドウ横幅
	unsigned int GetHeight(void) const {
		return height;
	}

private:
	// ウィンドウ
	std::unique_ptr<Window>win;

	// ウィンドウ横幅
	unsigned int width;

	// ウィンドウ縦幅
	unsigned int height;
};
