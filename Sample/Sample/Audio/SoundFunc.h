#pragma once
#include <vector>
namespace sound {
	// 8�r�b�g���m����
	void LoadMono8(std::vector<float>& data, FILE* file);

	// 16�r�b�g���m����
	void LoadMono16(std::vector<float>& data, FILE* file);

	// 8�r�b�g�X�e���I
	void LoadStereo8(std::vector<float>& data, FILE* file);

	// 16�r�b�g�X�e���I
	void LoadStereo16(std::vector<float>& data, FILE* file);

	// ���O�v�Z
	int Log(const int& bottom, const int& value);

	// �n�j���O��
	std::vector<float> HanningWindow(const int& size);

	// ���U�t�[���G�ϊ�
	void DFT(const std::vector<float>& waveData, std::vector<float>& real, std::vector<float>& imag, const int& size);

	// �����t�[���G�ϊ�
	void FFT(const std::vector<float>& waveData, std::vector<float>& real, std::vector<float>& imag, const int& size);
}
