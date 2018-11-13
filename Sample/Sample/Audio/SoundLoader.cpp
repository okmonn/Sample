#include "SoundLoader.h"
#include "../etc/Func.h"
#include <tchar.h>

// スレッド最大数
#define THREAD_MAX 5

// short型のオーバーフローの防止
#define OVERFLLOW_SHORT 32768.0f

// char型のオーバーフローの防止
#define OVERFLLOW_CHAR 127.0f

// ステレオ8ビット
struct Stereo8 {
	unsigned char left;
	unsigned char right;

	void operator=(const int& i) {
		left = i;
		right = i;
	}
};

// ステレオ16ビット
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

// コンストラクタ
SoundLoader::SoundLoader() : 
	threadFlag(true)
{
	sound.clear();
	th.resize(THREAD_MAX);
}

// デストラクタ
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

// 読み込み
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

	//拡張部分の読み込み
	std::vector<unsigned char>extended(fmt.size - (sizeof(fmt) - sizeof(fmt.id) - sizeof(fmt.size)));
	if (extended.size() > 0)
	{
		fread(extended.data(), sizeof(unsigned char) * extended.size(), 1, sound[fileName].file);
	}

	//ダミー宣言 
	std::string chunkID;
	chunkID.resize(4);

	while (true)
	{
		fread(&chunkID[0], sizeof(unsigned char) * chunkID.size(), 1, sound[fileName].file);

		//DATAチャンク発見
		if (chunkID == "data")
		{
			break;
		}
		//その他のチャンク
		else
		{
			unsigned long size = 0;
			fread(&size, sizeof(size), 1, sound[fileName].file);

			std::vector<unsigned char>data(size);
			fread(data.data(), sizeof(unsigned char) * data.size(), 1, sound[fileName].file);
		}
	}

	//DATAの読み込み 
	data.id = chunkID;
	fread(&data.size, sizeof(data.size), 1, sound[fileName].file);

	sound[fileName].channel = fmt.channel;
	sound[fileName].sample  = fmt.sample;
	sound[fileName].bit     = fmt.bit;

	//別スレッドで読み込み
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

// 8ビットモノラル
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

		//float値に変換・音データを0～2の範囲に正規化
		i = static_cast<float>(tmp) / OVERFLLOW_CHAR;
	}
}

// 16ビットモノラル
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

		//float値に変換・音データを-1～1の範囲に正規化
		i = static_cast<float>(tmp) / OVERFLLOW_SHORT;
	}
}

// 8ビットステレオ
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

		//float値に変換・音データを0～2の範囲に正規化
		data[i]     = static_cast<float>(tmp.left)  / OVERFLLOW_CHAR;
		data[i + 1] = static_cast<float>(tmp.right) / OVERFLLOW_CHAR;
	}
}

// 16ビットステレオ
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

		//float値に変換
		data[i]     = static_cast<float>(tmp.left)  / OVERFLLOW_SHORT;
		data[i + 1] = static_cast<float>(tmp.right) / OVERFLLOW_SHORT;
	}
}

// 非同期読み込み
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

// 削除
void SoundLoader::Delete(const std::string & fileName)
{
	if (sound.find(fileName) != sound.end())
	{
		sound.erase(sound.find(fileName));
	}
}
