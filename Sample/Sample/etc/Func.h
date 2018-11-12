#pragma once
#include <string>

namespace func {
	// ワイド文字変換
	std::wstring ChangeWString(const std::string& string);

	// 文字列の確認
	bool CheckChar(const std::string& find, unsigned char* data, const unsigned int& dataSize);
}
