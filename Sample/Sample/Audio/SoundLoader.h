#pragma once
#include <map>
#include <vector>
#include <string>
#include <memory>
#include <thread>
#include <functional>

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
		std::shared_ptr<std::vector<std::vector<float>>>data;
	};

public:
	// デストラクタ
	~SoundLoader();

	// インスタンス変数の取得
	static SoundLoader& Get(void) {
		static SoundLoader instance;
		return instance;
	}

	// 読み込み
	int Load(const std::string& fileName);

	// 削除
	void Delete(const std::string& fileName);

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
	std::shared_ptr<std::vector<std::vector<float>>>GetData(const std::string& fileName) {
		return sound[fileName].data;
	}

	// 読み込み確認フラグの取得
	bool GetFlag(const std::string& fileName) {
		return flag[fileName];
	}

private:
	// コンストラクタ
	SoundLoader();
	SoundLoader(const SoundLoader&) {
	}
	void operator=(const SoundLoader&) {
	}

	// 読み込みテーブルのセット
	void SetTbl(void);

	// 非同期読み込み
	void LoadStream(const std::string& fileName);


	// スレッドフラグ
	bool threadFlag;

	// データ
	std::map<std::string, SoundData>sound;

	// スレッド
	std::vector<std::thread>th;

	// 読み込み完了フラグ
	std::map<std::string, bool>flag;

	// 
	std::map<int, std::map<int, std::function<void(std::vector<float>& tmp, FILE* file)>>>load;
};
