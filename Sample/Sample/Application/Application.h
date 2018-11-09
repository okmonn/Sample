#pragma once
#include "../Input/KeyData.h"
#include <memory>

class Window;
class Input;
class Union;

class Application
{
public:
	// �R���X�g���N�^
	Application(const unsigned int& width = 640, const unsigned int& height = 480);
	// �f�X�g���N�^
	~Application();

	// ���b�Z�[�W�̊m�F
	bool CheckMsg(void);

	// �L�[�̓���
	bool CheckKey(const int& i);

	// �`��E����
	void Render(void);

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
	std::unique_ptr<Window>win;

	// �C���v�b�g
	std::unique_ptr<Input>input;

	// ���j�I��
	std::unique_ptr<Union>un;

	// �E�B���h�E�n���h��
	static void* winHandle;

	// �E�B���h�E����
	static unsigned int width;

	// �E�B���h�E�c��
	static unsigned int height;
};
