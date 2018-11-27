#pragma once
#include <string>
#include <memory>

class Window;
class Device;
class Root;
class Pipe;
class Effector;
class Sound;

class Application
{
public:
	//デストラクタ
	~Application();

	// インスタンス変数の取得
	static Application& Get(void) {
		static Application instance;
		return instance;
	}

	// メッセージの確認
	bool CheckMsg(void);

	// サウンドファイルの読み込み
	void Load(const std::string& fileName);

	// 再生
	void Play(const bool& loop = false);

	// 停止
	void Stop(void);

private:
	// コンストラクタ
	Application();
	Application(const Application&) {
	}
	void operator=(const Application&) {
	}

	// クラスの生成
	void Create(void);


	// ウィンドウ
	std::shared_ptr<Window>win;
	// デバイス
	std::shared_ptr<Device>dev;

	// ルートシグネチャ
	std::shared_ptr<Root>root;

	// パイプライン
	std::shared_ptr<Pipe>pipe;

	// エフェクター
	std::shared_ptr<Effector>effect;

	// サウンド
	std::shared_ptr<Sound>sound;
};
