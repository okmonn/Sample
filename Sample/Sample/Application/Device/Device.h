#pragma once

struct ID3D12Device;
struct IDXGIFactory4;
struct IDXGIAdapter1;

class Device
{
public:
	// �R���X�g���N�^
	Device();
	// �f�X�g���N�^
	~Device();

	// �f�o�C�X�̎擾
	ID3D12Device* GetDev(void) const {
		return dev;
	}
	// �t�@�N�g���[�̎擾
	IDXGIFactory4* GetFactory(void) const {
		return factory;
	}
	// �A�_�v�^�[�̎擾
	IDXGIAdapter1* GetAdapter(void) const {
		return adapter;
	}
 
private:
	// �t�@�N�g���[�̐���
	long CreateFactory(void);

	// �f�o�C�X�̐���
	long CreateDev(void);

	// ������
	void Init(void);


	// �f�o�C�X
	ID3D12Device* dev;

	// �t�@�N�g���[
	IDXGIFactory4* factory;

	// �A�_�v�^�[
	IDXGIAdapter1* adapter;
};
