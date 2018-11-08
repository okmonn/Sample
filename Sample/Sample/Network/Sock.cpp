#include "Sock.h"
#include <Windows.h>
#include <tchar.h>

#pragma comment(lib,"ws2_32.lib")

// �R���X�g���N�^
Sock::Sock() : 
	sock(INVALID_SOCKET)
{
	data = {};
	addr = {};
	fds = {};
	time = { 10, 0 };
	readfds = {};
}

// �f�X�g���N�^
Sock::~Sock()
{
}

// �X�^�[�g�A�b�v
int Sock::StartUp(void)
{
	auto hr = WSAStartup(MAKEWORD(2, 0), &data);
	if (hr != 0)
	{
		OutputDebugString(_T("\nWSA�̃X�^�[�g�A�b�v�F���s\n"));
	}

	return hr;
}

// �\�P�b�g�̐���
int Sock::CreateSock(void)
{
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		OutputDebugString(_T("\n�\�P�b�g�̐����F���s\n"));
		return -1;
	}

	return 0;
}

// �ʐM�̏I��
void Sock::Finish(void)
{
	if (sock != INVALID_SOCKET)
	{
		closesocket(sock);
	}

	WSACleanup();
}
