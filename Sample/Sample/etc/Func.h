#pragma once
#include <string>

namespace func {
	// ���C�h�����ϊ�
	std::wstring ChangeWString(const std::string& string);

	// ������̊m�F
	bool CheckChar(const std::string& find, unsigned char* data, const unsigned int& dataSize);
}
