#include "Server.h"
#include "Information.h"
#include <Windows.h>
#include <tchar.h>

// �N���C�A���g�ő吔
#define CLIENT_MAX 10

// �R���X�g���N�^
Server::Server() 
{
	client.resize(CLIENT_MAX);
	clientAddr.resize(CLIENT_MAX);
	memset(client.data(), INVALID_SOCKET, sizeof(SOCKET) * client.size());

	Init();
}

// �f�X�g���N�^
Server::~Server()
{
	for (unsigned int i = 0; i < CLIENT_MAX; ++i)
	{
		Close(i);
	}
	Finish();
}

// �ʐM�̏���
int Server::SetUp(void)
{
	addr.sin_family           = AF_INET;
	addr.sin_port             = htons(PORT);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;

	int flag = 1;
	auto hr = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&flag, sizeof(flag));
	if (hr != 0)
	{
		OutputDebugString(_T("\n�\�P�b�g�̐ݒ�F���s\n"));
	}

	return hr;
}

// �A�h���X�Ƃ̊֘A�t��
int Server::Bind(void)
{
	auto hr = bind(sock, (sockaddr*)&addr, sizeof(addr));
	if (hr != 0)
	{
		OutputDebugString(_T("\n�A�h���X�Ƃ̊֘A�t���F���s\n"));
	}

	return hr;
}

// �N���C�A���g�Ƃ̐ڑ��ҋ@
int Server::Listen(void)
{
	auto hr = listen(sock, 5);
	if (hr != 0)
	{
		OutputDebugString(_T("\n�N���C�A���g�Ƃ̐ڑ��ҋ@�F���s\n"));
	}

	return hr;
}

// ������
void Server::Init(void)
{
	StartUp();
	CreateSock();
	SetUp();
	Bind();
	Listen();
}

// �N���C�A���g�̐ڑ�
int Server::Accept(void)
{
	FD_ZERO(&readfds);

	int size = sizeof(sockaddr_in);

	for (unsigned int i = 0; i < CLIENT_MAX; ++i)
	{
		if (client[i] != INVALID_SOCKET)
		{
			continue;
		}

		client[i] = accept(sock, (sockaddr*)&clientAddr[i], &size);
		if (client[i] == INVALID_SOCKET)
		{
			OutputDebugString(_T("\n�N���C�A���g�Ƃ̐ڑ��F���s\n"));
			return -1;
		}
		else
		{
			FD_SET(client[i], &readfds);
			break;
		}
	}

	return 0;
}

// ���M
int Server::Send(const char * data, int size)
{
	auto hr = 0;
	for (unsigned int i = 0; i < CLIENT_MAX; ++i)
	{
		if (client[i] == INVALID_SOCKET)
		{
			continue;
		}

		hr = send(client[i], data, size, 0);
		if (hr != size)
		{
			OutputDebugString(_T("\n���M�F���s\n"));
			break;
		}
	}

	return hr;
}

// ��M
int Server::Recv(std::vector<char> & data, const unsigned int & index)
{
	data.clear();
	data.shrink_to_fit();

	memcpy(&fds, &readfds, sizeof(fd_set));

	if (select(0, &fds, nullptr, nullptr, &time) == 0
		|| select(0, &fds, nullptr, nullptr, &time) == -1)
	{
		OutputDebugString(_T("\n��M�F���s\n"));
		return -1;
	}

	if (client[index] == INVALID_SOCKET)
	{
		OutputDebugString(_T("\n���݂��܂���\n"));
		return -1;
	}

	if (FD_ISSET(client[index], &fds))
	{
		char dummy[1024];
		memset(&dummy[0], 0, sizeof(dummy));

		while (true)
		{
			auto hr = recv(client[index], dummy, sizeof(dummy), 0);
			if (hr == -1)
			{
				Close(index);
				break;
			}
			else if (hr <= 0)
			{
				break;
			}

			for (int i = 0; i < hr; ++i)
			{
				data.push_back(dummy[i]);
			}
		}
	}

	return 0;
}

// �N���C�A���g�Ƃ̐ڑ��I��
int Server::Close(const unsigned int & index)
{
	if (index >= CLIENT_MAX)
	{
		return -1;
	}

	if (client[index] != INVALID_SOCKET)
	{
		closesocket(client[index]);
		client[index]     = INVALID_SOCKET;
		clientAddr[index] = {};
	}

	return 0;
}
