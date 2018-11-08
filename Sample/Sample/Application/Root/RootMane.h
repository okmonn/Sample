#pragma once
#include "../../etc/tString.h"
#include <map>
#include <memory>

class Device;
class Root;

class RootMane
{
public:
	// コンストラクタ
	RootMane(std::weak_ptr<Device>dev);
	// デストラクタ
	~RootMane();

	// ルートシグネチャクラスの生成
	void CreateRoot(int& i, const std::tstring& fileName);

	// ルートシグネチャクラスの取得
	std::shared_ptr<Root>Get(int& i) {
		return root[&i];
	}

private:
	// デバイス
	std::weak_ptr<Device>dev;

	// ルートシグネチャ
	std::map<int*, std::shared_ptr<Root>>root;
};