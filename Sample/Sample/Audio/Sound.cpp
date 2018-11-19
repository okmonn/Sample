#include "Sound.h"
#include "SoundLoader.h"
#include "XAudio2.h"
#include "VoiceCallback.h"
#include <xaudio2.h>
#include <ks.h>
#include <ksmedia.h>
#include <tchar.h>
#include "SoundFunc.h"

// ����}�N��
#define Destroy(X) { if((X) != nullptr) (X)->DestroyVoice(); (X) = nullptr; }

// �o�b�t�@�̍ő吔
#define BUFFER_MAX 3

// �X�s�[�J�[�ݒ�p�z��
const DWORD spk[] = {
	KSAUDIO_SPEAKER_MONO,
	KSAUDIO_SPEAKER_STEREO,
	KSAUDIO_SPEAKER_STEREO | SPEAKER_LOW_FREQUENCY,
	KSAUDIO_SPEAKER_QUAD,
	0,
	KSAUDIO_SPEAKER_5POINT1,
	0,
	KSAUDIO_SPEAKER_7POINT1_SURROUND
};

// �R���X�g���N�^
Sound::Sound(std::weak_ptr<XAudio2>audio) :
	loader(SoundLoader::Get()), audio(audio), voice(nullptr), 
	index(0), loop(false), threadFlag(true)
{
	callback = std::make_shared<VoiceCallback>();
}

// �f�X�g���N�^
Sound::~Sound()
{
	threadFlag = false;
	if (th.joinable() == true)
	{
		th.join();
	}

	Destroy(voice);
}

// �\�[�X�{�C�X�̐���
long Sound::CreateVoice(const std::string & fileName)
{
	WAVEFORMATEXTENSIBLE desc{};
	desc.Format.cbSize          = sizeof(WAVEFORMATEXTENSIBLE) - sizeof(WAVEFORMATEX);
	desc.Format.nChannels       = loader.GetChannel(fileName);
	desc.Format.nSamplesPerSec  = loader.GetSample(fileName);
	desc.Format.nBlockAlign     = sizeof(float) * desc.Format.nChannels;
	desc.Format.wBitsPerSample  = sizeof(float) * 8;
	desc.Format.nAvgBytesPerSec = desc.Format.nSamplesPerSec * desc.Format.nBlockAlign;
	desc.Format.wFormatTag      = WAVE_FORMAT_EXTENSIBLE;

	desc.dwChannelMask               = spk[desc.Format.nChannels - 1];
	desc.Samples.wValidBitsPerSample = desc.Format.wBitsPerSample;
	desc.SubFormat                   = KSDATAFORMAT_SUBTYPE_IEEE_FLOAT;

	auto hr = audio.lock()->Get()->CreateSourceVoice(&voice, (WAVEFORMATEX*)(&desc), 0, 1.0f, &(*callback));
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n�\�[�X�{�C�X�̐����F���s\n"));
	}

	return hr;
}

// �ǂݍ���
void Sound::Load(const std::string & fileName)
{
	loader.Load(fileName);
	data = loader.GetData(fileName);

	CreateVoice(fileName);

	name = fileName;

	th = std::thread(&Sound::UpData, this);
}

// �񓯊�����
void Sound::UpData(void)
{
	XAUDIO2_VOICE_STATE st{};
	while (threadFlag)
	{
		if (data.lock()->size() <= BUFFER_MAX
			|| index >= data.lock()->size()
			|| data.lock()->at(index).size() <= 0)
		{
			continue;
		}

		voice->GetState(&st);
		if (st.BuffersQueued > BUFFER_MAX)
		{
			continue;
		}

		XAUDIO2_BUFFER buf{};
		buf.AudioBytes = sizeof(float) * data.lock()->at(index).size();
		buf.pAudioData = (unsigned char*)(data.lock()->at(index).data());

		auto hr = voice->SubmitSourceBuffer(&buf);
		if (FAILED(hr))
		{
			OutputDebugString(_T("\n�o�b�t�@�̒ǉ��F���s\n"));
			continue;
		}

		if (index + 1 >= data.lock()->size() && loader.GetFlag(name) == true)
		{
			if (loop == false)
			{
				Stop();
			}
			index = 0;
		}
		else
		{
			++index;
		}
	}
}

// �Đ�
long Sound::Play(const bool & loop)
{
	auto hr = voice->Start();
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n�Đ��F���s\n"));
		return hr;
	}

	this->loop = loop;

	return hr;
}

// ��~
long Sound::Stop(void)
{
	auto hr = voice->Stop();
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n��~�F���s\n"));
	}

	return hr;
}

// ���Z�b�g
void Sound::Reset(void)
{
	index = 0;
}
