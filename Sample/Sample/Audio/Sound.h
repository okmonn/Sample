#pragma once
#include <map>
#include <string>
#include <vector>
#include <memory>
#include <thread>

struct IXAudio2SourceVoice;
class SoundLoader;
class XAudio2;
class VoiceCallback;

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

	// �ǂݍ��݃t�@�C�����̎擾
	std::string GetName(void) const {
		return name;
	}
	// �I���t���O�̎擾
	bool GetEnd(void) const {
		return end;
	}
	// �R�[���o�b�N�̎擾
	std::shared_ptr<VoiceCallback>GetCallback(void) const {
		return callback;
	}

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

	// �R�[���o�b�N
	std::shared_ptr<VoiceCallback>callback;

	// �y�Ȗ�
	std::string name;

	// �o�b�t�@�z��ԍ�
	unsigned int index;

	// �I���t���O
	bool end;

	// ���[�v�t���O
	bool loop;

	// �X���b�h�t���O
	bool threadFlag;

	// �g�`�f�[�^
	std::weak_ptr<std::vector<std::vector<float>>>data;

	// �X���b�h
	std::thread th;
};
