#pragma once
#include "../tString.h"

class Window
{
public:
	// コンストラクタ　
	Window(const unsigned int& width = 640, const unsigned int& height = 480);
	// デストラクタ
	~Window();

	// タイトル名の変更
	void ChangeTitle(const std::tstring& title);

	// ウィンドウハンドルの取得
	void* Get(void) const {
		return handle;
	}

private:
	// ウィンドウのコールバック
#if _WIN64
	static long __stdcall WindowProc(void* hWnd, unsigned int message, unsigned __int64 wParam, __int64 lParam);
#else
	static long __stdcall WindowProc(void* hWnd, unsigned int message, long wParam, long lParam);
#endif

	// ウィンドウの生成
	void Create(const unsigned int& width, const unsigned int& height);


	// ウィンドウハンドル
	void* handle;

	// ウィンドウインスタンス
	void* instance;

	// ウィンドウ名
	const wchar_t* name;
};
