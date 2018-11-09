#pragma once
#include "../etc/tString.h"

class Window
{
public:
	// コンストラクタ　
	Window(void** handle);
	// デストラクタ
	~Window();

	// タイトル名の変更
	void ChangeTitle(void** handle, const std::tstring& title);

private:
	// ウィンドウのコールバック
	static long __stdcall WindowProc(void* hWnd, unsigned int message, long wParam, long lParam);

	// ウィンドウの生成
	void Create(void** handle);


	// ウィンドウインスタンス
	void* instance;

	// ウィンドウ名
	const wchar_t* name;
};
