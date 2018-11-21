#pragma once
#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <unordered_map>

struct IXAudio2SourceVoice;

class XAudio2;
class VoiceCallback;
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

	// 再生
	long Play(const bool& loop = false);

	// 停止
	long Stop(void);

	// コールバックハンドルの取得
	void* GetHandle(void) const;

private:
	// ソースボイスの生成
	long CreateVoice(const std::string& filName);

	// 非同期処理
	void Stream(void);


	// オーディオ
	XAudio2& audio;

	// ローダー
	SoundLoader& loader;
	
	// コールバック
	std::unique_ptr<VoiceCallback>call;

	// ソースボイス
	IXAudio2SourceVoice* voice;

	// ループフラグ
	bool loop;

	// 再生終了フラグ
	bool end;

	// スレッドフラグ
	bool threadFlag;

	// 配列番号
	unsigned int index;

	// 読み込みファイル名
	std::string name;

	// 非同期スレッド
	std::thread th;

	//波形データ
	std::weak_ptr<std::unordered_map<int, std::vector<float>>>wave;
};
