#pragma once
#include <vector>
namespace sound {
	// ログ計算
	int Log(const int& bottom, const int& value);

	// ハニング窓
	std::vector<float> HanningWindow(const int& size);

	// 離散フーリエ変換
	void DFT(const std::vector<float>& waveData, std::vector<float>& real, std::vector<float>& imag, const int& size);

	// 高速フーリエ変換
	void FFT(const std::vector<float>& waveData, std::vector<float>& real, std::vector<float>& imag, const int& size);
}
