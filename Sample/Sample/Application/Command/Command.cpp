#include "Command.h"
#include "../Queue/Queue.h"
#include "../List/List.h"

// コンストラクタ
Command::Command(std::weak_ptr<Device>dev, const D3D12_COMMAND_LIST_TYPE & type)
{
	queue = std::make_shared<Queue>(dev, type);
	list  = std::make_shared<List>(dev, type);
}

// デストラクタ
Command::~Command()
{
	list.reset();
	queue.reset();
}
