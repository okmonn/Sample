#include "SoundLoader.h"
#include "../etc/Func.h"
#include <tchar.h>

// �X���b�h�ő吔
#define THREAD_MAX 5

// short�^�̃I�[�o�[�t���[�̖h�~
#define OVERFLLOW_SHORT 32768.0f

// char�^�̃I�[�o�[�t���[�̖h�~
#define OVERFLLOW_CHAR 127.0f

// �X�e���I8�r�b�g
struct Stereo8 {
	unsigned char left;
	unsigned char right;

	void operator=(const int& i) {
		left = i;
		right = i;
	}
};

// �X�e���I16�r�b�g
struct Stereo16 {
	short left;
	short right;

	void operator=(const int& i) {
		left = i;
		right = i;
	}
};

// RIFF
struct RIFF {
	unsigned char id[4];
	long size;
	unsigned char type[4];
};

// FMT
struct FMT {
	unsigned char id[4];
	long size;
	short type;
	short channel;
	long sample;
	long byte;
	short block;
	short bit;
};

// DATA
struct DATA {
	std::string id;
	long size;
};

// �R���X�g���N�^
SoundLoader::SoundLoader() : 
	threadFlag(true)
{
	sound.clear();
	th.resize(THREAD_MAX);
}

// �f�X�g���N�^
SoundLoader::~SoundLoader()
{
	threadFlag = false;
	for (auto& i : th)
	{
		if (i.joinable() == true)
		{
			i.join();
		}
	}
}

// �ǂݍ���
int SoundLoader::Load(const std::string & fileName)
{
	if (fopen_s(&sound[fileName].file, fileName.c_str(), "rb") != 0)
	{
		return -1;
	}

	RIFF riff{};
	FMT  fmt{};
	DATA data{};

	fread(&riff, sizeof(RIFF), 1, sound[fileName].file);
	if (func::CheckChar("RIFF", riff.id, _countof(riff.id)) != true
		|| func::CheckChar("WAVE", riff.type, _countof(riff.type)) != true)
	{
		fclose(sound[fileName].file);
		return -1;
	}

	fread(&fmt, sizeof(FMT), 1, sound[fileName].file);
	if (func::CheckChar("fmt ", fmt.id, _countof(fmt.id)) != true)
	{
		fclose(sound[fileName].file);
		return -1;
	}

	//�g�������̓ǂݍ���
	std::vector<unsigned char>extended(fmt.size - (sizeof(fmt) - sizeof(fmt.id) - sizeof(fmt.size)));
	if (extended.size() > 0)
	{
		fread(extended.data(), sizeof(unsigned char) * extended.size(), 1, sound[fileName].file);
	}

	//�_�~�[�錾 
	std::string chunkID;
	chunkID.resize(4);

	while (true)
	{
		fread(&chunkID[0], sizeof(unsigned char) * chunkID.size(), 1, sound[fileName].file);

		//DATA�`�����N����
		if (chunkID == "data")
		{
			break;
		}
		//���̑��̃`�����N
		else
		{
			unsigned long size = 0;
			fread(&size, sizeof(size), 1, sound[fileName].file);

			std::vector<unsigned char>data(size);
			fread(data.data(), sizeof(unsigned char) * data.size(), 1, sound[fileName].file);
		}
	}

	//DATA�̓ǂݍ��� 
	data.id = chunkID;
	fread(&data.size, sizeof(data.size), 1, sound[fileName].file);

	sound[fileName].channel = fmt.channel;
	sound[fileName].sample  = fmt.sample;
	sound[fileName].bit     = fmt.bit;

	//�ʃX���b�h�œǂݍ���
	bool flag = true;
	while (flag)
	{
		for (auto itr = th.begin(); itr != th.end(); ++itr)
		{
			if (itr->joinable() == false)
			{
				sound[fileName].data = std::make_shared<std::map<int, std::vector<float>>>();
				*itr = std::thread(&SoundLoader::LoadStream, this, fileName);
				flag = false;
				break;
			}
		}
	}

	return 0;
}

// 8�r�b�g���m����
void SoundLoader::LoadMono8(std::vector<float>& data, FILE * file)
{
	unsigned char tmp = 0;
	for (auto& i : data)
	{
		if (feof(file) == 0)
		{
			fread(&tmp, sizeof(unsigned char), 1, file);
		}
		else
		{
			tmp = 0;
		}

		//float�l�ɕϊ��E���f�[�^��0�`2�͈̔͂ɐ��K��
		i = static_cast<float>(tmp) / OVERFLLOW_CHAR;
	}
}

// 16�r�b�g���m����
void SoundLoader::LoadMono16(std::vector<float>& data, FILE * file)
{
	short tmp = 0;
	for (auto& i : data)
	{
		if (feof(file) == 0)
		{
			fread(&tmp, sizeof(short), 1, file);
		}
		else
		{
			tmp = 0;
		}

		//float�l�ɕϊ��E���f�[�^��-1�`1�͈̔͂ɐ��K��
		i = static_cast<float>(tmp) / OVERFLLOW_SHORT;
	}
}

// 8�r�b�g�X�e���I
void SoundLoader::LoadStereo8(std::vector<float>& data, FILE * file)
{
	Stereo8 tmp = {};
	for (unsigned int i = 0; i < data.size(); i += 2)
	{
		if (feof(file) == 0)
		{
			fread(&tmp, sizeof(Stereo8), 1, file);
		}
		else
		{
			tmp = 0;
		}

		//float�l�ɕϊ��E���f�[�^��0�`2�͈̔͂ɐ��K��
		data[i]     = static_cast<float>(tmp.left)  / OVERFLLOW_CHAR;
		data[i + 1] = static_cast<float>(tmp.right) / OVERFLLOW_CHAR;
	}
}

// 16�r�b�g�X�e���I
void SoundLoader::LoadStereo16(std::vector<float>& data, FILE * file)
{
	Stereo16 tmp = {};
	for (unsigned int i = 0; i < data.size(); i += 2)
	{
		if (feof(file) == 0)
		{
			fread(&tmp, sizeof(Stereo16), 1, file);
		}
		else
		{
			tmp = 0;
		}

		//float�l�ɕϊ�
		data[i]     = static_cast<float>(tmp.left)  / OVERFLLOW_SHORT;
		data[i + 1] = static_cast<float>(tmp.right) / OVERFLLOW_SHORT;
	}
}

// �񓯊��ǂݍ���
void SoundLoader::LoadStream(const std::string & fileName)
{
	int read = 0;
	std::vector<float>tmp(sound[fileName].sample / ((sound[fileName].bit / 8) * sound[fileName].channel));
	if (tmp.size() % 2 != 0)
	{
		tmp.resize(tmp.size() + 1);
	}

	while (std::feof(sound[fileName].file) == 0 && threadFlag == true)
	{
		switch (sound[fileName].channel)
		{
		case 1:
			(sound[fileName].bit == 8) ? LoadMono8(tmp, sound[fileName].file) : LoadMono16(tmp, sound[fileName].file);
			break;
		case 2:
			(sound[fileName].bit == 8) ? LoadStereo8(tmp, sound[fileName].file) : LoadStereo16(tmp, sound[fileName].file);
			break;
		default:
			break;
		}

		sound[fileName].data->insert(std::make_pair(read, tmp));
		++read;
	}

	fclose(sound[fileName].file);
}

// �폜
void SoundLoader::Delete(const std::string & fileName)
{
	if (sound.find(fileName) != sound.end())
	{
		sound.erase(sound.find(fileName));
	}
}
