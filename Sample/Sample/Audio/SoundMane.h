#pragma once
#include <map>
#include <string>
#include <memory>

class XAudio2;
class Sound;

class SoundMane
{
public:
	// �R���X�g���N�^
	SoundMane();
	// �f�X�g���N�^
	~SoundMane();

	// �ǂݍ���
	void Load(const std::string& fileName, int& i);

	// �Đ�
	void Play(int& i, const bool& loop = false);

	// ��~
	void Stop(int& i);

	// �폜
	void Delete(int& i);

private:
	// �I�[�f�B�I
	std::shared_ptr<XAudio2>audio;

	// �T�E���h
	std::map<int*, std::unique_ptr<Sound>>sound;
};
