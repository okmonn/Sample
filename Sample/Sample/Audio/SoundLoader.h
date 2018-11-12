#pragma once
#include <map>
#include <vector>
#include <string>
#include <memory>
#include <thread>

struct _iobuf;
typedef _iobuf FILE;

class SoundLoader
{
	// �T�E���h�f�[�^
	struct SoundData {
		//�`�����l����
		int channel;
		//�T���v�����O���g��
		int sample;
		//�ʎq���r�b�g��
		int bit;
		//�t�@�C��
		FILE* file;
		//�g�`�f�[�^
		std::shared_ptr<std::map<int, std::vector<float>>>data;
	};

public:
	// �R���X�g���N�^
	SoundLoader();
	// �f�X�g���N�^
	~SoundLoader();

	// �ǂݍ���
	int Load(const std::string& fileName);

	// �`���l�����̎擾
	int GetChannel(const std::string& fileName) {
		return sound[fileName].channel;
	}
	// �T���v�����O���g���̎擾
	int GetSample(const std::string& fileName) {
		return sound[fileName].sample;
	}
	// �ʎq���r�b�g���̎擾
	int GetBit(const std::string& fileName) {
		return sound[fileName].bit;
	}
	// �g�`�f�[�^�̎擾
	std::shared_ptr<std::map<int, std::vector<float>>>GetData(const std::string& fileName) {
		return sound[fileName].data;
	}

private:
	// 8�r�b�g���m����
	void LoadMono8(std::vector<float>& data, FILE* file);

	// 16�r�b�g���m����
	void LoadMono16(std::vector<float>& data, FILE* file);

	// 8�r�b�g�X�e���I
	void LoadStereo8(std::vector<float>& data, FILE* file);

	// 16�r�b�g�X�e���I
	void LoadStereo16(std::vector<float>& data, FILE* file);

	// �񓯊��ǂݍ���
	void LoadStream(const std::string& fileName);


	// �X���b�h�t���O
	bool threadFlag;

	// �f�[�^
	std::map<std::string, SoundData>sound;

	// �X���b�h
	std::vector<std::thread>th;
};
