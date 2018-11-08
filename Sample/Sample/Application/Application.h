#pragma once
#include <memory>

class Window;

class Application
{
public:
	// �R���X�g���N�^
	Application(const unsigned int& width = 640, const unsigned int& height = 480);
	// �f�X�g���N�^
	~Application();

	// ���b�Z�[�W�̊m�F
	bool CheckMsg(void);

	// �E�B���h�E�̃R�[���o�b�N
	static long __stdcall WindowProc(void* hWnd, unsigned int message, long wParam, long lParam);

	// �E�B���h�E�n���h���̎擾
	static void* GetWinHandle(void) {
		return winHandle;
	}
	// �E�B���h�E�c���̎擾
	static unsigned int GetWidth(void) {
		return width;
	}
	// �E�B���h�E����
	static unsigned int GetHeight(void) {
		return height;
	}

private:
	// �E�B���h�E
	std::shared_ptr<Window>win;

	// �E�B���h�E�n���h��
	static void* winHandle;

	// �E�B���h�E����
	static unsigned int width;

	// �E�B���h�E�c��
	static unsigned int height;
};
