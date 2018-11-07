#pragma once

class Application
{
public:
	// コンストラクタ
	Application();
	// デストラクタ
	~Application();

	// ウィンドウハンドルの取得
	static void* GetHwnd(void) {
		return handle;
	}

protected:
	// ウィンドウのコールバック
	static long* __stdcall WindowProc(void* hWnd, unsigned int message, unsigned int* wParam, long* lParam);

private:
	// ウィンドウハンドル
	static void* handle;

	// ウィンドウインスタンス
	void* instance;

	// ウィンドウ名
	wchar_t* name;
};
