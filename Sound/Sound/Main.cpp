#include "Application/Application.h"

#ifdef _DEBUG
int main()
#else
int __stdcall WinMain(void* hInstance, void* hPrevInstance, char* lpCmdLine, int nShowCmd)
#endif
{
	auto& a = Application::Get();
	a.Load("animal.wav");
	a.Play();

	while ( a.CheckMsg())
	{

	}

	return 0;
}