#pragma once
#include <map>
#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <functional>

class SoundLoader
{
	struct Wave {
		//�t�@�C���n���h��
		FILE* file;
		//�`�����l����
		int channel;
		//�T���v�����O���g��
		int sample;
		//�ʎq���r�b�g��
		int bit;
		//�g�`���
		std::shared_ptr<std::vector<std::vector<float>>>data;
	};

public:
	// �f�X�g���N�^
	~SoundLoader();

	// �C���X�^���X�ϐ��̎擾
	static SoundLoader& Get(void) {
		static SoundLoader instance;
		return instance;
	}

	// �ǂݍ���
	int Load(const std::string& fileName);

	// �`�����l�����̎擾
	int GetChannel(const std::string& fileName) {
		return wave[fileName].channel;
	}
	// �T���v�����O���g���̎擾
	int GetSample(const std::string& fileName) {
		return wave[fileName].sample;
	}
	// �ʎq���r�b�g���̎擾
	int GetBit(const std::string& fileName) {
		return wave[fileName].bit;
	}
	// �g�`�f�[�^�̎擾
	std::shared_ptr<std::vector<std::vector<float>>> GetWave(const std::string& fileName) {
		return wave[fileName].data;
	}

private:
	// �R���X�g���N�^
	SoundLoader();
	SoundLoader(const SoundLoader&) {
	}
	void operator=(const SoundLoader&) {
	}

	// �ǂݍ��݃e�[�u���̐���
	void InitTbl(void);

	// �񓯊��ǂݍ���
	void Stream(const std::string& fileName);


	// �X���b�h�I���t���O
	bool threadFlag;

	// �g�`���
	std::map<std::string, Wave>wave;

	// �g�`�ǂݍ��݊����t���O
	std::map<std::string, bool>flag;

	// �ǂݍ��݃X���b�h
	std::vector<std::thread>th;

	// �ǂݍ��݃e�[�u��
	std::map<int, std::map<int, std::function<void(std::vector<float>& tmp, FILE* file)>>>load;
};
