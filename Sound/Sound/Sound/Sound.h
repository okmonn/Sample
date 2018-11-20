#pragma once
#include <string>
#include <vector>
#include <memory>
#include <thread>

struct IXAudio2SourceVoice;

class XAudio2;
class SoundLoader;

class Sound
{
public:
	// �R���X�g���N�^
	Sound();
	// �f�X�g���N�^
	~Sound();

	// �ǂݍ���
	void Load(const std::string& fileName);

private:
	// �\�[�X�{�C�X�̐���
	long CreateVoice(const std::string& filName);

	// �񓯊�����
	void Stream(void);


	// �I�[�f�B�I
	XAudio2& audio;

	// ���[�_�[
	SoundLoader& loader;

	// �\�[�X�{�C�X
	IXAudio2SourceVoice* voice;

	// �X���b�h�t���O
	bool threadFlag;

	// �񓯊��X���b�h
	std::thread th;

	//�g�`�f�[�^
	std::weak_ptr<std::vector<std::vector<float>>>wave;
};
