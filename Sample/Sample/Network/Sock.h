#pragma once
#include <WinSock2.h>
#include <vector>

class Sock
{
public:
	// �R���X�g���N�^
	Sock();
	// �f�X�g���N�^
	virtual ~Sock();

protected:
	// �X�^�[�g�A�b�v
	int StartUp(void);

	// �\�P�b�g�̐���
	int CreateSock(void);

	// �ʐM�̏I��
	void Finish(void);


	// �f�[�^
	WSADATA data;

	// �����̃\�P�b�g
	SOCKET sock;

	// �ڑ���A�h���X
	sockaddr_in addr;

	// �Z���N�g�Ŏg�p
	fd_set fds;

	// �Z���N�g�Ŏg�p
	fd_set readfds;

	// �^�C���A�E�g
	timeval time;
};
