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
	// サウンドデータ
	struct SoundData {
		//チャンネル数
		int channel;
		//サンプリング周波数
		int sample;
		//量子化ビット数
		int bit;
		//ファイル
		FILE* file;
		//波形データ
		std::shared_ptr<std::map<int, std::vector<float>>>data;
	};

public:
	// コンストラクタ
	SoundLoader();
	// デストラクタ
	~SoundLoader();

	// 読み込み
	int Load(const std::string& fileName);

	// チャネル数の取得
	int GetChannel(const std::string& fileName) {
		return sound[fileName].channel;
	}
	// サンプリング周波数の取得
	int GetSample(const std::string& fileName) {
		return sound[fileName].sample;
	}
	// 量子化ビット数の取得
	int GetBit(const std::string& fileName) {
		return sound[fileName].bit;
	}
	// 波形データの取得
	std::shared_ptr<std::map<int, std::vector<float>>>GetData(const std::string& fileName) {
		return sound[fileName].data;
	}

private:
	// 8ビットモノラル
	void LoadMono8(std::vector<float>& data, FILE* file);

	// 16ビットモノラル
	void LoadMono16(std::vector<float>& data, FILE* file);

	// 8ビットステレオ
	void LoadStereo8(std::vector<float>& data, FILE* file);

	// 16ビットステレオ
	void LoadStereo16(std::vector<float>& data, FILE* file);

	// 非同期読み込み
	void LoadStream(const std::string& fileName);


	// スレッドフラグ
	bool threadFlag;

	// データ
	std::map<std::string, SoundData>sound;

	// スレッド
	std::vector<std::thread>th;
};
