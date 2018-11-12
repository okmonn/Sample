#include "RootMane.h"
#include "../Device/Device.h"
#include "Root.h"
#include "RootCompute.h"

#pragma comment (lib, "d3dcompiler.lib")

// コンストラクタ
RootMane::RootMane(std::weak_ptr<Device>dev) : 
	dev(dev)
{
	root.clear();
	compute.clear();
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

// ルートシグネチャコンピュートクラスの生成
void RootMane::CreateRootCompute(int & i, const std::tstring & fileName)
{
	compute[&i] = std::make_shared<RootCompute>(dev, fileName);
}
