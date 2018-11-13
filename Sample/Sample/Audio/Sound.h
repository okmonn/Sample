#pragma once
#include <map>
#include <string>
#include <vector>
#include <memory>
#include <thread>

struct IXAudio2SourceVoice;
class SoundLoader;
class XAudio2;

class Sound
{
public:
	// �R���X�g���N�^
	Sound(std::weak_ptr<XAudio2>audio);
	// �f�X�g���N�^
	~Sound();

	// �ǂݍ���
	void Load(const std::string& fileName);

	// �Đ�
	long Play(const bool& loop);

	// ��~
	long Stop(void);

	// ���Z�b�g
	void Reset(void);

private:
	// �\�[�X�{�C�X�̐���
	long CreateVoice(const std::string& fileName);

	// �񓯊�����
	void UpData(void);


	// �T�E���h���[�_�[
	SoundLoader& loader;

	// �I�[�f�B�I
	std::weak_ptr<XAudio2>audio;

	// �\�[�X�{�C�X
	IXAudio2SourceVoice* voice;

	// �o�b�t�@�z��ԍ�
	unsigned int index;

	// ���[�v�t���O
	bool loop;

	// �X���b�h�t���O
	bool threadFlag;

	// �g�`�f�[�^
	std::weak_ptr<std::map<int, std::vector<float>>>data;

	// �X���b�h
	std::thread th;
};
