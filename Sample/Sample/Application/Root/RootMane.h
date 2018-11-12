#pragma once
#include "../../etc/tString.h"
#include <map>
#include <memory>

class Device;
class Root;
class RootCompute;

class RootMane
{
public:
	// コンストラクタ
	RootMane(std::weak_ptr<Device>dev);
	// デストラクタ
	~RootMane();

	// ルートシグネチャクラスの生成
	void CreateRoot(int& i, const std::tstring& fileName);

	// ルートシグネチャコンピュートクラスの生成
	void CreateRootCompute(int& i, const std::tstring& fileName);

	// ルートシグネチャクラスの取得
	std::shared_ptr<Root>Get(int& i) {
		return root[&i];
	}
	// ルートシグネチャコンピュートクラスの取得
	std::shared_ptr<RootCompute>GetCompute(int& i) {
		return compute[&i];
	}

private:
	// デバイス
	std::weak_ptr<Device>dev;

	// ルートシグネチャ
	std::map<int*, std::shared_ptr<Root>>root;

	// ルートシグネチャ・コンピュート
	std::map<int*, std::shared_ptr<RootCompute>>compute;
};