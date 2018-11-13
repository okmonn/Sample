#pragma once
#include <map>
#include <string>
#include <memory>

class XAudio2;
class Sound;

class SoundMane
{
public:
	// コンストラクタ
	SoundMane();
	// デストラクタ
	~SoundMane();

	// 読み込み
	void Load(const std::string& fileName, int& i);

	// 再生
	void Play(int& i, const bool& loop = false);

	// 停止
	void Stop(int& i);

	// 削除
	void Delete(int& i);

private:
	// オーディオ
	std::shared_ptr<XAudio2>audio;

	// サウンド
	std::map<int*, std::unique_ptr<Sound>>sound;
};
