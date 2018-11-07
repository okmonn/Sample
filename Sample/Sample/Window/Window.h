#pragma once

class Window
{
public:
	// �R���X�g���N�^�@
	Window(const unsigned int& width, const unsigned int& height);
	// �f�X�g���N�^
	~Window();

	// �^�C�g�����̕ύX
	void ChangeTitle(const wchar_t* title);

	// �E�B���h�E�n���h���̎擾
	void* Get(void) const {
		return handle;
	}
	
private:
	// �E�B���h�E�̐���
	void Create(const unsigned int& width, const unsigned int& height);

	// �n���h��
	void* handle;

	// �E�B���h�E�C���X�^���X
	void* instance;

	// �E�B���h�E��
	const wchar_t* name;
};
