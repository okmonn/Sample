#pragma once
#include <map>
#include <string>
#include <memory>
#include <DirectXMath.h>

struct ID3D12GraphicsCommandList;
struct D3D12_SUBRESOURCE_DATA;
struct ID3D12Resource;
class DescriptorMane;
class TextureLoad;
class Device;
class List;
class Root;
class Pipe;

namespace tex {
	// ���_
	struct Vertex {
		//���W
		DirectX::XMFLOAT3 pos;
		//uv
		DirectX::XMFLOAT2 uv;
	};
}

class Texture
{
	//�摜�f�[�^
	struct Tex {
		//�摜���\�[�X
		ID3D12Resource* rsc;
		//�f�R�[�h
		unsigned char* decode;
		//�T�u�f�[�^
		std::weak_ptr<D3D12_SUBRESOURCE_DATA>sub;
		//���_���\�[�XID
		int vRsc;
		//���_���M�f�[�^
		unsigned int* data;
		//�o���h�����X�g
		std::unique_ptr<List>list;
	};

public:
	// �R���X�g���N�^
	Texture(std::weak_ptr<Device>dev, std::weak_ptr<Root>root, std::weak_ptr<Pipe>pipe);
	// �f�X�g���N�^
	~Texture();

	// �ǂݍ���
	void Load(const std::string& fileName, int& i);

	// �`��
	void Draw(ID3D12GraphicsCommandList* list, int& i);

	// �폜
	void Delete(int& i);

private:
	// �V�F�[�_�[���\�[�X�r���[�̐���
	void CreateShaderView(int* i);

	// �T�u���\�[�X�ɏ�������
	long WriteSub(int* i);

	// ���_���\�[�X�̐���
	long CreateVertexRsc(int* i);

	// ���_�}�b�v
	long Map(int* i);

	// �o���h���̃Z�b�g
	void SetBundle(int* i);


	// �f�B�X�N���v�^�[�}�l�[�W���[
	DescriptorMane& descMane;

	// �e�N�X�`�����[�_�[
	TextureLoad& loader;

	// �f�o�C�X
	std::weak_ptr<Device>dev;

	// ���[�g�V�O�l�`��
	std::weak_ptr<Root>root;

	// �p�C�v���C��
	std::weak_ptr<Pipe>pipe;

	// �摜�f�[�^
	std::map<int*, Tex>tex;
};
