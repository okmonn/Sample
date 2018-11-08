#pragma once
#include "Sock.h"

class Client :
	public Sock
{
public:
	// �R���X�g���N�^
	Client();
	// �f�X�g���N�^
	~Client();

	// ���M
	int Send(const char* data, int size);

	// ��M
	int Recv(std::vector<char>& data);

private:
	// �ڑ�
	int Connect(void);

	// �ʐM�̏���
	int SetUp(void);

	// ������
	void Init(void);
};
