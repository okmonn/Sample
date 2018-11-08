#pragma once
#include<memory>

enum D3D12_COMMAND_LIST_TYPE : int;
struct ID3D12CommandQueue;
struct ID3D12CommandList;
class Device;

class Queue
{
public:
	// �R���X�g���N�^
	Queue(std::weak_ptr<Device>dev, const D3D12_COMMAND_LIST_TYPE& type);
	// �f�X�g���N�^
	~Queue();

	// �R�}���h���X�g�̎��s
	void Execute(ID3D12CommandList** list, const unsigned int& num = 1);

	// �R�}���h�L���[�̎擾
	ID3D12CommandQueue* Get(void) const {
		return queue;
	}

private:
	// �R�}���h�L���[�̐���
	long Create(const D3D12_COMMAND_LIST_TYPE& type);


	// �f�o�C�X
	std::weak_ptr<Device>dev;

	// �R�}���h�L���[
	ID3D12CommandQueue* queue;
};
