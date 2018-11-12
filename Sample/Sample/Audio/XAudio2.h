#pragma once

struct IXAudio2;
struct IXAudio2MasteringVoice;

class XAudio2
{
public:
	// コンストラクタ
	XAudio2();
	// デストラクタ
	~XAudio2();

	// オーディオの取得
	IXAudio2* Get(void) const {
		return audio;
	}
	// マスタリングの取得
	IXAudio2MasteringVoice* GetMastering(void) const {
		return mastering;
	}

private:
	// COMの初期化
	long InitCom(void);

	// オーディオの生成
	long CreateAudio(void);

	// マスタリングの生成
	long CreateMastering(void);

	// 初期化
	void Init(void);


	// オーディオ
	IXAudio2* audio;

	// マスタリング
	IXAudio2MasteringVoice* mastering;
};
