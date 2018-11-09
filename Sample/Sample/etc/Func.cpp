#include "Func.h"
#include <Windows.h>

// ���C�h�����ϊ�
std::wstring func::ChangeWString(const std::string & string)
{
	auto byteSize = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, string.c_str(), -1, nullptr, 0);

	std::wstring wstr;
	wstr.resize(byteSize);

	//�ϊ�
	byteSize = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, string.c_str(), -1, &wstr[0], byteSize);

	return wstr;
}
