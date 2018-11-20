#pragma once
#include <string>
#include <vector>
#include <memory>
#include <thread>

struct IXAudio2SourceVoice;

class XAudio2;
class SoundLoader;

class Sound
{
public:
	// コンストラクタ
	Sound();
	// デストラクタ
	~Sound();

	// 読み込み
	void Load(const std::string& fileName);

private:
	// ソースボイスの生成
	long CreateVoice(const std::string& filName);

	// 非同期処理
	void Stream(void);


	// オーディオ
	XAudio2& audio;

	// ローダー
	SoundLoader& loader;

	// ソースボイス
	IXAudio2SourceVoice* voice;

	// スレッドフラグ
	bool threadFlag;

	// 非同期スレッド
	std::thread th;

	//波形データ
	std::weak_ptr<std::vector<std::vector<float>>>wave;
};
