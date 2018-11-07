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

	// �E�B���h�E�c���̎擾
	unsigned int GetWidth(void) const {
		return width;
	}
	// �E�B���h�E����
	unsigned int GetHeight(void) const {
		return height;
	}

private:
	// �E�B���h�E
	std::unique_ptr<Window>win;

	// �E�B���h�E����
	unsigned int width;

	// �E�B���h�E�c��
	unsigned int height;
};
