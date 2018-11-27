#pragma once
#include <string>
#include <memory>

class Window;
class Device;
class Root;
class Pipe;
class Effector;
class Sound;

class Application
{
public:
	//�f�X�g���N�^
	~Application();

	// �C���X�^���X�ϐ��̎擾
	static Application& Get(void) {
		static Application instance;
		return instance;
	}

	// ���b�Z�[�W�̊m�F
	bool CheckMsg(void);

	// �T�E���h�t�@�C���̓ǂݍ���
	void Load(const std::string& fileName);

	// �Đ�
	void Play(const bool& loop = false);

	// ��~
	void Stop(void);

private:
	// �R���X�g���N�^
	Application();
	Application(const Application&) {
	}
	void operator=(const Application&) {
	}

	// �N���X�̐���
	void Create(void);


	// �E�B���h�E
	std::shared_ptr<Window>win;
	// �f�o�C�X
	std::shared_ptr<Device>dev;

	// ���[�g�V�O�l�`��
	std::shared_ptr<Root>root;

	// �p�C�v���C��
	std::shared_ptr<Pipe>pipe;

	// �G�t�F�N�^�[
	std::shared_ptr<Effector>effect;

	// �T�E���h
	std::shared_ptr<Sound>sound;
};
