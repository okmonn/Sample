#include "Application.h"
#include "../Window/Window.h"
#include "Device/Device.h"
#include "Queue/Queue.h"
#include "List/List.h"
#include <d3d12.h>

#pragma comment(lib, "d3d12.lib")

std::shared_ptr<Device>dev;
std::shared_ptr<Queue>q;
std::shared_ptr<List>list;

// �R���X�g���N�^
Application::Application(const unsigned int & width, const unsigned int & height) : 
	win(std::make_unique<Window>(width, height)), width(width), height(height)
{
#ifdef _DEBUG
	ID3D12Debug* debug = nullptr;
	D3D12GetDebugInterface(IID_PPV_ARGS(&debug));
	debug->EnableDebugLayer();
#endif
	dev = std::make_shared<Device>();
	q = std::make_shared<Queue>(dev, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);
	list = std::make_shared<List>(dev, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);
}

// �f�X�g���N�^
Application::~Application()
{
}

// ���b�Z�[�W�̊m�F
bool Application::CheckMsg(void)
{
	static MSG msg{};

	if (msg.message == WM_QUIT)
	{
		return false;
	}
	else
	{
		//�Ăяo�����X���b�h�����L���Ă���E�B���h�E�ɑ��M���ꂽ���b�Z�[�W�ۗ̕�����Ă��镨���擾
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			//���z�L�[���b�Z�[�W�𕶎����b�Z�[�W�ɕϊ�
			TranslateMessage(&msg);
			//1�̃E�B�h�E�v���V�[�W���Ƀ��b�Z�[�W�𑗏o����
			DispatchMessage(&msg);
		}
	}

	return true;
}

// �E�B���h�E�̃R�[���o�b�N
long Application::WindowProc(void * hWnd, unsigned int message, long wParam, long lParam)
{
	switch (message)
	{
	case WM_CREATE:
		//// Save the DXSample* passed in to CreateWindow.
		//LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
		//SetWindowLongPtr(reinterpret_cast<HWND>(hWnd), GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
		break;

	case WM_PAINT:
		// ����
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	default:
		break;
	}

	return DefWindowProc((HWND)hWnd, message, wParam, lParam);
}


