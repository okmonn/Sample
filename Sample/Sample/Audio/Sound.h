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
	// コンストラクタ
	Sound(std::weak_ptr<XAudio2>audio);
	// デストラクタ
	~Sound();

	// 読み込み
	void Load(const std::string& fileName);

	// 再生
	long Play(const bool& loop);

	// 停止
	long Stop(void);

	// リセット
	void Reset(void);

	// 読み込みファイル名の取得
	std::string GetName(void) const {
		return name;
	}
	// 終了フラグの取得
	bool GetEnd(void) const {
		return end;
	}
	// コールバックの取得
	std::shared_ptr<VoiceCallback>GetCallback(void) const {
		return callback;
	}

private:
	// ソースボイスの生成
	long CreateVoice(const std::string& fileName);

	// 非同期処理
	void UpData(void);


	// サウンドローダー
	SoundLoader& loader;

	// オーディオ
	std::weak_ptr<XAudio2>audio;

	// ソースボイス
	IXAudio2SourceVoice* voice;

	// コールバック
	std::shared_ptr<VoiceCallback>callback;

	// 楽曲名
	std::string name;

	// バッファ配列番号
	unsigned int index;

	// 終了フラグ
	bool end;

	// ループフラグ
	bool loop;

	// スレッドフラグ
	bool threadFlag;

	// 波形データ
	std::weak_ptr<std::vector<std::vector<float>>>data;

	// スレッド
	std::thread th;
};
