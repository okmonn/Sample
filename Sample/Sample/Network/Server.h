#pragma once
#include "Sock.h"

class Server :
	public Sock
{
public:
	// �R���X�g���N�^
	Server();
	// �f�X�g���N�^
	~Server();

	// �N���C�A���g�̐ڑ�
	int Accept(void);

	// ���M
	int Send(const char* data, int size);

	// ��M
	int Recv(std::vector<char>& data, const unsigned int& index = 0);

private:
	// �ʐM�̏���
	int SetUp(void);

	// �A�h���X�Ƃ̊֘A�t��
	int Bind(void);

	// �N���C�A���g�Ƃ̐ڑ��ҋ@
	int Listen(void);

	// ������
	void Init(void);

	// �N���C�A���g�Ƃ̐ڑ��I��
	int Close(const unsigned int& index = 0);


	// �N���C�A���g�\�P�b�g
	std::vector<SOCKET>client;

	// �N���C�A���g�̃A�h���X
	std::vector<sockaddr_in>clientAddr;
};
