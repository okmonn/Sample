#include "Application/Application.h"

// エントリーポイント
#ifdef _DEBUG
int main()
#else
int __stdcall WinMain(void* hInstance, void* hPrevInstance, char* lpCmdLine, int nShowCmd)
#endif
{
	Application a(640, 480);

	while (a.CheckMsg() && a.CheckKey(INPUT_ESCAPE) == false)
	{
		a.Render();
	}

	return 0;
}