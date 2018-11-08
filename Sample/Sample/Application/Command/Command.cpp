#include "Command.h"
#include "../Queue/Queue.h"
#include "../List/List.h"

// �R���X�g���N�^
Command::Command(std::weak_ptr<Device>dev, const D3D12_COMMAND_LIST_TYPE & type)
{
	queue = std::make_shared<Queue>(dev, type);
	list  = std::make_shared<List>(dev, type);
}

// �f�X�g���N�^
Command::~Command()
{
	list.reset();
	queue.reset();
}
