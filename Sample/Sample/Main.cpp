#include "Application/Application.h"

// �G���g���[�|�C���g
int main()
{
	Application a(640, 480);

	while (a.CheckMsg() && a.CheckKey(INPUT_ESCAPE) == false)
	{
		a.Render();
	}

	return 0;
}