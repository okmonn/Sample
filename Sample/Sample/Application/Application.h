#pragma once

class Application
{
public:
	// �R���X�g���N�^
	Application();
	// �f�X�g���N�^
	~Application();

	// �E�B���h�E�n���h���̎擾
	static void* GetHwnd(void) {
		return handle;
	}

protected:
	// �E�B���h�E�̃R�[���o�b�N
	static long* __stdcall WindowProc(void* hWnd, unsigned int message, unsigned int* wParam, long* lParam);

private:
	// �E�B���h�E�n���h��
	static void* handle;

	// �E�B���h�E�C���X�^���X
	void* instance;

	// �E�B���h�E��
	wchar_t* name;
};
