#include "Func.h"
#include <Windows.h>

// ワイド文字変換
std::wstring func::ChangeWString(const std::string & string)
{
	auto byteSize = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, string.c_str(), -1, nullptr, 0);

	std::wstring wstr;
	wstr.resize(byteSize);

	//変換
	byteSize = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, string.c_str(), -1, &wstr[0], byteSize);

	return wstr;
}

// 文字列の確認
bool func::CheckChar(const std::string & find, unsigned char * data, const unsigned int & dataSize)
{
	int index = 0;
	for (unsigned int i = 0; i < dataSize; ++i)
	{
		if (find[index] == data[i])
		{
			if (index + 1 < find.size())
			{
				++index;
			}
			else
			{
				return true;
			}
		}
	}

	return false;
}
