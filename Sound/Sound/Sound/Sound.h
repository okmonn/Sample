#pragma once
#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <unordered_map>

struct IXAudio2SourceVoice;

class XAudio2;
class VoiceCallback;
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

	// �Đ�
	long Play(const bool& loop = false);

	// ��~
	long Stop(void);

	// �R�[���o�b�N�n���h���̎擾
	void* GetHandle(void) const;

private:
	// �\�[�X�{�C�X�̐���
	long CreateVoice(const std::string& filName);

	// �񓯊�����
	void Stream(void);


	// �I�[�f�B�I
	XAudio2& audio;

	// ���[�_�[
	SoundLoader& loader;
	
	// �R�[���o�b�N
	std::unique_ptr<VoiceCallback>call;

	// �\�[�X�{�C�X
	IXAudio2SourceVoice* voice;

	// ���[�v�t���O
	bool loop;

	// �Đ��I���t���O
	bool end;

	// �X���b�h�t���O
	bool threadFlag;

	// �z��ԍ�
	unsigned int index;

	// �ǂݍ��݃t�@�C����
	std::string name;

	// �񓯊��X���b�h
	std::thread th;

	//�g�`�f�[�^
	std::weak_ptr<std::unordered_map<int, std::vector<float>>>wave;
};
