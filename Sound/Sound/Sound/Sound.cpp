#include "Sound.h"
#include "XAudio2/XAudio2.h"
#include "XAudio2/VoiceCallback.h"
#include "SoundLoader/SoundLoader.h"
#include "Destroy.h"
#include <ks.h>
#include <ksmedia.h>

// バッファの最大数
#define BUF_MAX 3

// スピーカー設定用配列
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

// コンストラクタ
Sound::Sound() : 
	audio(XAudio2::Get()), loader(SoundLoader::Get()), 
	voice(nullptr), loop(false), end(false), threadFlag(true), index(0)
{
	call = std::make_unique<VoiceCallback>();
}

// デストラクタ
Sound::~Sound()
{
	threadFlag = false;
	if (th.joinable() == true)
	{
		th.join();
	}

	Destroy(voice);
}

// ソースボイスの生成
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
		OutputDebugString(_T("\nソースボイスの生成：失敗\n"));
	}

	return hr;
}

// 読み込み
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

// 非同期処理
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

		if (wave.lock()->find(index) == wave.lock()->end())
		{
			continue;
		}

		XAUDIO2_BUFFER buf{};
		buf.AudioBytes = sizeof(float) * wave.lock()->at(index).size();
		buf.pAudioData = (unsigned char*)wave.lock()->at(index).data();

		auto hr = voice->SubmitSourceBuffer(&buf);
		if (FAILED(hr))
		{
			OutputDebugString(_T("\nバッファの追加：失敗\n"));
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

// 再生
long Sound::Play(const bool & loop)
{
	auto hr = voice->Start();
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n再生：失敗\n"));
		return hr;
	}

	this->loop = loop;

	return hr;
}

// 停止
long Sound::Stop(void)
{
	auto hr = voice->Stop();
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n停止：失敗\n"));
	}

	return hr;
}

// コールバックハンドルの取得
void * Sound::GetHandle(void) const
{
	return call->handle;
}

