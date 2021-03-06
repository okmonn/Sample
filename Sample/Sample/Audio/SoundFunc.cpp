#include "SoundFunc.h"
#include <math.h>

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

// 円周率
#define PI 3.14159265f

// short型のオーバーフローの防止
#define OVERFLLOW_SHORT 32768.0f

// char型のオーバーフローの防止
#define OVERFLLOW_CHAR 127.0f


// 8ビットモノラル
void sound::LoadMono8(std::vector<float>& data, FILE * file)
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

		//float値に変換・音データを-1〜1の範囲に正規化
		i = static_cast<float>(tmp) / OVERFLLOW_CHAR - 1.0f;
	}
}

// 16ビットモノラル
void sound::LoadMono16(std::vector<float>& data, FILE * file)
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

		//float値に変換・音データを-1〜1の範囲に正規化
		i = static_cast<float>(tmp) / OVERFLLOW_SHORT;
	}
}

// 8ビットステレオ
void sound::LoadStereo8(std::vector<float>& data, FILE * file)
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

		//float値に変換・音データを-1〜1の範囲に正規化
		data[i] = static_cast<float>(tmp.left) / OVERFLLOW_CHAR - 1.0f;
		data[i + 1] = static_cast<float>(tmp.right) / OVERFLLOW_CHAR - 1.0f;
	}
}

// 16ビットステレオ
void sound::LoadStereo16(std::vector<float>& data, FILE * file)
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
		data[i] = static_cast<float>(tmp.left) / OVERFLLOW_SHORT;
		data[i + 1] = static_cast<float>(tmp.right) / OVERFLLOW_SHORT;
	}
}

// ログ計算
int sound::Log(const int & bottom, const int & value)
{
	return static_cast<int>(log(value) / log(bottom));
}

// ハニング窓
std::vector<float> sound::HanningWindow(const int & size)
{
	std::vector<float>tmp(size);
	
	//偶数
	if (size % 2 == 0)
	{
		for (int i = 0; i < size; ++i)
		{
			tmp[i] = 0.5f - 0.5f * cosf(2.0f * PI * i / size);
		}
	}
	//奇数
	else
	{
		for (int i = 0; i < size; ++i)
		{
			tmp[i] = 0.5f - 0.5f * cosf(2.0f * PI * (i + 0.5f) / size);
		}
	}

	return tmp;
}

// 離散フーリエ変換
void sound::DFT(const std::vector<float>& waveData, std::vector<float>& real, std::vector<float> & imag, const int & size)
{
	real.resize(size, 0.0f);
	imag.resize(size, 0.0f);

	// ダミー宣言
	float r = 0.0f;
	float c = 0.0f;

	auto hanning = sound::HanningWindow(size);

	for (int i = 0; i < size; ++i)
	{
		for (int n = 0; n < size; ++n)
		{
			r = cosf(2.0f * PI * i * n / size);
			c = -sinf(2.0f * PI * i * n / size);

			real[i] += r * (waveData[i] * hanning[i]);
			imag[i] += c * (waveData[i] * hanning[i]);
		}
	}
}

// 高速フーリエ変換
void sound::FFT(const std::vector<float>& waveData, std::vector<float>& real, std::vector<float>& imag, const int & size)
{
	real.resize(size);
	imag.resize(size);

	auto stage = std::ceilf((float)log2(size));

	std::vector<float>dummy = waveData;

	dummy.resize((int)pow(2, stage));

	//ダミー宣言
	int tmp1, tmp2, tmp3;
	float real1, real2, real3;
	float imag1, imag2, imag3;
	std::vector<int>index(size, 0);

	for (int s = 1; s <= stage; ++s)
	{
		for (int i = 0; i < pow(2, s - 1); ++i)
		{
			if ((int)pow(2, s - 1) + i >= size || index[i] + (int)pow(2, stage - s) >= size)
			{
				continue;
			}

			index[(int)pow(2, s - 1) + i] = index[i] + (int)pow(2, stage - s);

			for (int n = 0; n < pow(2, stage - s); ++n)
			{
				tmp1 = (int)pow(2, stage - s + 1) * i + n;
				tmp2 = (int)pow(2, stage - s) + tmp1;
				tmp3 = (int)pow(2, s - 1) * n;

				real1 = dummy[tmp1];
				real2 = dummy[tmp2];
				real3 = cosf((2.0f * PI * tmp3) / size);

				imag1 = 0.0f;
				imag2 = 0.0f;
				imag3 = -sinf((2.0f * PI * tmp3) / size);

				if (tmp1 >= size || tmp2 >= size)
				{
					continue;
				}

				if (s < stage)
				{
					real[tmp1] = real1 + real2;
					real[tmp2] = (real1 - real2) * real3;

					imag[tmp1] = imag1 + imag2;
					imag[tmp2] = (imag1 - imag2) * real3;
				}
				else
				{
					real[tmp1] = real1 + real2;
					real[tmp2] = real1 - real2;

					imag[tmp1] = imag1 + imag2;
					imag[tmp2] = imag1 - imag2;
				}
			}
		}
	}

	float r_dummy;
	float i_dummy;

	//インデックスの並び替え
	for (int i = 0; i < size; ++i)
	{
		if (index[i] > i)
		{
			r_dummy = real[index[i]];
			i_dummy = imag[index[i]];

			real[index[i]] = real[i];
			imag[index[i]] = imag[i];

			real[i] = r_dummy;
			imag[i] = i_dummy;
		}
	}
}
