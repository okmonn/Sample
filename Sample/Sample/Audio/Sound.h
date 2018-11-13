#pragma once
#include <map>
#include <string>
#include <vector>
#include <memory>
#include <thread>

struct IXAudio2SourceVoice;
class SoundLoader;
class XAudio2;

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

	// バッファ配列番号
	unsigned int index;

	// ループフラグ
	bool loop;

	// スレッドフラグ
	bool threadFlag;

	// 波形データ
	std::weak_ptr<std::map<int, std::vector<float>>>data;

	// スレッド
	std::thread th;
};
