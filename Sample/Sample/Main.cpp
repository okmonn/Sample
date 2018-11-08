#include "Application/Application.h"

// エントリーポイント
int main()
{
	Application a(640, 480);

	while (a.CheckMsg() && a.CheckKey(INPUT_ESCAPE) == false)
	{

	}

	return 0;
}