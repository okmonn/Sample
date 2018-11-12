#pragma once
#include "../etc/tString.h"

class Window
{
public:
	// �R���X�g���N�^�@
	Window(void** handle);
	// �f�X�g���N�^
	~Window();

	// �^�C�g�����̕ύX
	void ChangeTitle(void** handle, const std::tstring& title);

private:
	// �E�B���h�E�̃R�[���o�b�N
#if _WIN64
	static long __stdcall WindowProc(void* hWnd, unsigned int message, unsigned __int64 wParam, __int64 lParam);
#else
	static long __stdcall WindowProc(void* hWnd, unsigned int message, long wParam, long lParam);
#endif

	// �E�B���h�E�̐���
	void Create(void** handle);


	// �E�B���h�E�C���X�^���X
	void* instance;

	// �E�B���h�E��
	const wchar_t* name;
};
