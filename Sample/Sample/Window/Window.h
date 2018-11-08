#pragma once

class Window
{
public:
	// コンストラクタ　
	Window(void** handle);
	// デストラクタ
	~Window();

	// タイトル名の変更
	void ChangeTitle(void** handle, const wchar_t* title);

private:
	// ウィンドウの生成
	void Create(void** handle);


	// ウィンドウインスタンス
	void* instance;

	// ウィンドウ名
	const wchar_t* name;
};
