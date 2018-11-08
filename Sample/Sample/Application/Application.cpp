#include "Application.h"
#include "../Window/Window.h"
#include "Device/Device.h"
#include "Queue/Queue.h"
#include "List/List.h"
#include "Swap/Swap.h"
#include "Render/Render.h"
#include "Fence/Fence.h"
#include <d3d12.h>
#include <dxgi1_6.h>

#pragma comment(lib, "d3d12.lib")

void* Application::winHandle     = nullptr;
unsigned int Application::width  = 0;
unsigned int Application::height = 0;

std::shared_ptr<Device>dev;
std::shared_ptr<Queue>q;
std::shared_ptr<List>list;
std::shared_ptr<Swap>swap;
std::shared_ptr<Render>ren;
std::shared_ptr<Fence>fen;

// コンストラクタ
Application::Application(const unsigned int & width, const unsigned int & height)
{
#ifdef _DEBUG
	ID3D12Debug* debug = nullptr;
	D3D12GetDebugInterface(IID_PPV_ARGS(&debug));
	debug->EnableDebugLayer();
#endif
	this->width  = width;
	this->height = height;

	win = std::make_shared<Window>(&winHandle);
	dev = std::make_shared<Device>();
	q = std::make_shared<Queue>(dev, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);
	list = std::make_shared<List>(dev, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);
	swap = std::make_shared<Swap>(dev, q);
	ren = std::make_shared<Render>(dev, list, swap);
	fen = std::make_shared<Fence>(dev, q);
}

// デストラクタ
Application::~Application()
{
}

// メッセージの確認
bool Application::CheckMsg(void)
{
	static MSG msg{};

	if (msg.message == WM_QUIT)
	{
		return false;
	}
	else
	{
		//呼び出し側スレッドが所有しているウィンドウに送信されたメッセージの保留されている物を取得
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			//仮想キーメッセージを文字メッセージに変換
			TranslateMessage(&msg);
			//1つのウィドウプロシージャにメッセージを送出する
			DispatchMessage(&msg);
		}
	}

	return true;
}

// ウィンドウのコールバック
long Application::WindowProc(void * hWnd, unsigned int message, long wParam, long lParam)
{
	static float color[] = { 1,1,1,1 };
	switch (message)
	{
	case WM_CREATE:
		//// Save the DXSample* passed in to CreateWindow.
		//LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
		//SetWindowLongPtr(reinterpret_cast<HWND>(hWnd), GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
		break;

	case WM_PAINT:
		// 処理
		list->Reset(nullptr);
		list->SetViewport();
		list->SetScissor();
		list->SetBarrier(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET,
			ren->GetRsc(swap->Get()->GetCurrentBackBufferIndex()));
		
		ren->SetRender(nullptr, color);
		list->SetBarrier(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT,
			ren->GetRsc(swap->Get()->GetCurrentBackBufferIndex()));

		list->GetList()->Close();
		
		q->Get()->ExecuteCommandLists(1, (ID3D12CommandList**)(list->GetList()));
		swap->Present();
		fen->Wait();
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	default:
		break;
	}

	return DefWindowProc((HWND)hWnd, message, wParam, lParam);
}


