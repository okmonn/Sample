#pragma once
#include <memory>

enum D3D12_COMMAND_LIST_TYPE : int;
class Device;
class List;
class Queue;

class Command
{
public:
	// �R���X�g���N�^
	Command(std::weak_ptr<Device>dev, const D3D12_COMMAND_LIST_TYPE& type = static_cast<D3D12_COMMAND_LIST_TYPE>(0));
	// �f�X�g���N�^
	~Command();

	// �R�}���h�L���[�N���X�̎擾
	std::shared_ptr<Queue>GetQueue(void) const {
		return queue;
	}
	// �R�}���h���X�g�N���X�̎擾
	std::shared_ptr<List>GetList(void) const {
		return list;
	}

private:
	// �R�}���h�L���[
	std::shared_ptr<Queue>queue;

	// �R�}���h���X�g
	std::shared_ptr<List>list;
};
