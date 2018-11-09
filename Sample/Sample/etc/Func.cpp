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
