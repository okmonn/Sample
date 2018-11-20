#include "RootMane.h"
#include "../Device/Device.h"
#include "Root.h"
#include "RootCompute.h"

#pragma comment (lib, "d3dcompiler.lib")

// コンストラクタ
RootMane::RootMane()
{
	root.clear();
	compute.clear();
}

// デストラクタ
RootMane::~RootMane()
{
}

// ルートシグネチャクラスの生成
void RootMane::CreateRoot(int& i, std::weak_ptr<Device>dev, const std::tstring & fileName)
{
	root[&i] = std::make_shared<Root>(dev, fileName);
}

// ルートシグネチャコンピュートクラスの生成
void RootMane::CreateRootCompute(int & i, std::weak_ptr<Device>dev, const std::tstring & fileName)
{
	compute[&i] = std::make_shared<RootCompute>(dev, fileName);
}
