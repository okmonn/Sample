#include "SoundMane.h"
#include "XAudio2.h"
#include "Sound.h"

// �R���X�g���N�^
SoundMane::SoundMane()
{
	audio = std::make_shared<XAudio2>();

	sound.clear();
}

// �f�X�g���N�^
SoundMane::~SoundMane()
{
}

// �ǂݍ���
void SoundMane::Load(const std::string & fileName, int & i)
{
	sound[&i] = std::make_unique<Sound>(audio);
	sound[&i]->Load(fileName);
}

// �Đ�
void SoundMane::Play(int & i, const bool & loop)
{
	if (sound.find(&i) == sound.end())
	{
		return;
	}

	sound[&i]->Play(loop);
}

// ��~
void SoundMane::Stop(int & i)
{
	if (sound.find(&i) == sound.end())
	{
		return;
	}

	sound[&i]->Stop();
}

// �폜
void SoundMane::Delete(int & i)
{
	if (sound.find(&i) != sound.end())
	{
		sound.erase(&i);
	}
}
