#include "Sound.h"
#include "XAudio2/XAudio2.h"
#include "XAudio2/VoiceCallback.h"
#include "SoundLoader/SoundLoader.h"
#include "Destroy.h"
#include <ks.h>
#include <ksmedia.h>

// �o�b�t�@�̍ő吔
#define BUF_MAX 3

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
Sound::Sound() : 
	audio(XAudio2::Get()), loader(SoundLoader::Get()), 
	voice(nullptr), loop(false), end(false), threadFlag(true), index(0)
{
	call = std::make_unique<VoiceCallback>();
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
long Sound::CreateVoice(const std::string& fileName)
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

	auto hr = audio.GetAudio()->CreateSourceVoice(&voice, (WAVEFORMATEX*)(&desc), 0, 1.0f, &(*call));
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

	CreateVoice(fileName);

	name = fileName;

	wave = loader.GetWave(fileName);

	if (th.joinable() == false)
	{
		th = std::thread(&Sound::Stream, this);
	}
}

// �񓯊�����
void Sound::Stream(void)
{
	XAUDIO2_VOICE_STATE st{};

	while (threadFlag)
	{
		if (loader.GetWave(name)->size() <= BUF_MAX)
		{
			continue;
		}

		voice->GetState(&st);
		if (st.BuffersQueued >= BUF_MAX)
		{
			continue;
		}

		XAUDIO2_BUFFER buf{};
		buf.AudioBytes = sizeof(float) * wave.lock()->at(index).size();
		buf.pAudioData = (unsigned char*)wave.lock()->at(index).data();

		auto hr = voice->SubmitSourceBuffer(&buf);
		if (FAILED(hr))
		{
			OutputDebugString(_T("\n�o�b�t�@�̒ǉ��F���s\n"));
			continue;
		}

		if (index + 1 >= wave.lock()->size() && loader.GetFlag(name) == true)
		{
			if (loop == false)
			{
				Stop();
			}
			index = 0;
			end = true;
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

// �R�[���o�b�N�n���h���̎擾
void * Sound::GetHandle(void) const
{
	return call->handle;
}
