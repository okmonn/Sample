#pragma once

struct IXAudio2;
struct IXAudio2MasteringVoice;

class XAudio2
{
public:
	// �R���X�g���N�^
	XAudio2();
	// �f�X�g���N�^
	~XAudio2();

	// �I�[�f�B�I�̎擾
	IXAudio2* Get(void) const {
		return audio;
	}
	// �}�X�^�����O�̎擾
	IXAudio2MasteringVoice* GetMastering(void) const {
		return mastering;
	}

private:
	// COM�̏�����
	long InitCom(void);

	// �I�[�f�B�I�̐���
	long CreateAudio(void);

	// �}�X�^�����O�̐���
	long CreateMastering(void);

	// ������
	void Init(void);


	// �I�[�f�B�I
	IXAudio2* audio;

	// �}�X�^�����O
	IXAudio2MasteringVoice* mastering;
};
