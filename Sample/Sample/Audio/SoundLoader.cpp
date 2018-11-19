#include "SoundLoader.h"
#include "SoundFunc.h"
#include "../etc/Func.h"
#include <tchar.h>

// スレッド最大数
#define THREAD_MAX 5

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
	flag.clear();
	sound.clear();
	th.resize(THREAD_MAX);

	SetTbl();
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

// 読み込みテーブルのセット
void SoundLoader::SetTbl(void)
{
	load[1][8] = [&](std::vector<float>& tmp, FILE* file)->void {
		sound::LoadMono8(tmp, file);
	};

	load[1][16] = [&](std::vector<float>& tmp, FILE* file)->void {
		sound::LoadMono16(tmp, file);
	};

	load[2][8] = [&](std::vector<float>& tmp, FILE* file)->void {
		sound::LoadStereo8(tmp, file);
	};

	load[2][16] = [&](std::vector<float>& tmp, FILE* file)->void {
		sound::LoadStereo16(tmp, file);
	};
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
				sound[fileName].data = std::make_shared<std::vector<std::vector<float>>>();
				*itr = std::thread(&SoundLoader::LoadStream, this, fileName);
				flag = false;
				break;
			}
		}
	}

	return 0;
}

// 非同期読み込み
void SoundLoader::LoadStream(const std::string & fileName)
{
	flag[fileName] = false;

	std::vector<float>tmp((sound[fileName].sample * ((sound[fileName].bit / 8) * sound[fileName].channel)) / 100);
	if (tmp.size() % 2 != 0)
	{
		tmp.resize(tmp.size() + 1);
	}

	while (std::feof(sound[fileName].file) == 0 && threadFlag == true)
	{
		load[sound[fileName].channel][sound[fileName].bit](tmp, sound[fileName].file);

		sound[fileName].data->emplace_back(tmp);
	}

	fclose(sound[fileName].file);

	flag[fileName] = true;
}

// 削除
void SoundLoader::Delete(const std::string & fileName)
{
	if (sound.find(fileName) != sound.end())
	{
		sound.erase(sound.find(fileName));
	}
}
