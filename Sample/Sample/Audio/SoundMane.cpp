#include "SoundMane.h"
#include "XAudio2.h"
#include "Sound.h"

// コンストラクタ
SoundMane::SoundMane()
{
	audio = std::make_shared<XAudio2>();

	sound.clear();
}

// デストラクタ
SoundMane::~SoundMane()
{
}

// 読み込み
void SoundMane::Load(const std::string & fileName, int & i)
{
	sound[&i] = std::make_unique<Sound>(audio);
	sound[&i]->Load(fileName);
}

// 再生
void SoundMane::Play(int & i, const bool & loop)
{
	if (sound.find(&i) == sound.end())
	{
		return;
	}

	sound[&i]->Play(loop);
}

// 停止
void SoundMane::Stop(int & i)
{
	if (sound.find(&i) == sound.end())
	{
		return;
	}

	sound[&i]->Stop();
}

// 削除
void SoundMane::Delete(int & i)
{
	if (sound.find(&i) != sound.end())
	{
		sound.erase(&i);
	}
}
