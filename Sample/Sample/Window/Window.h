#pragma once

class Window
{
public:
	// コンストラクタ　
	Window(const unsigned int& width, const unsigned int& height);
	// デストラクタ
	~Window();

	// タイトル名の変更
	void ChangeTitle(const wchar_t* title);

	// ウィンドウハンドルの取得
	void* Get(void) const {
		return handle;
	}
	
private:
	// ウィンドウの生成
	void Create(const unsigned int& width, const unsigned int& height);

	// ハンドル
	void* handle;

	// ウィンドウインスタンス
	void* instance;

	// ウィンドウ名
	const wchar_t* name;
};
