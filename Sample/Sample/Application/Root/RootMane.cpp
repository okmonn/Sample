#include "RootMane.h"
#include "../Device/Device.h"
#include "Root.h"

// コンストラクタ
RootMane::RootMane(std::weak_ptr<Device>dev) : 
	dev(dev)
{
	root.clear();
}

// デストラクタ
RootMane::~RootMane()
{
}

// ルートシグネチャクラスの生成
void RootMane::CreateRoot(int& i, const std::tstring & fileName)
{
	root[&i] = std::make_shared<Root>(dev, fileName);
}
