#pragma once
#include <vector>
namespace sound {
	// ���O�v�Z
	int Log(const int& bottom, const int& value);

	// �n�j���O��
	std::vector<float> HanningWindow(const int& size);

	// ���U�t�[���G�ϊ�
	void DFT(const std::vector<float>& waveData, std::vector<float>& real, std::vector<float>& imag, const int& size);

	// �����t�[���G�ϊ�
	void FFT(const std::vector<float>& waveData, std::vector<float>& real, std::vector<float>& imag, const int& size);
}
