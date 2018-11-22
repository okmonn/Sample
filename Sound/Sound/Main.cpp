#include "Application/Application.h"
#include <Windows.h>

int main()
{
	auto& a = Application::Get();
	a.Load("animal.wav");
	a.Play();

	while (!(GetKeyState(VK_ESCAPE) & 0x80))
	{

	}

	return 0;
}