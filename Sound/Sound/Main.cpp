#include "Sound/Sound.h"

int main()
{
	Sound s;
	s.Load("sample.wav");
	s.Play();

	while (true)
	{

	}

	return 0;
}