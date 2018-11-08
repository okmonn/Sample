#pragma once
#include <map>
#include <string>
#include <memory>

struct D3D12_SUBRESOURCE_DATA;

class TextureLoad
{
	// ���f�[�^
	struct Origin {
		//�q�[�vID
		int heap;
		//�f�R�[�h
		std::unique_ptr<unsigned char[]>decode;
		//�T�u�f�[�^
		std::shared_ptr<D3D12_SUBRESOURCE_DATA>sub;
	};
public:
	// �R���X�g���N�^
	TextureLoad();
	// �f�X�g���N�^
	~TextureLoad();

	// �ǂݍ���
	long Load(const std::string& fileName);

private:
	// �I���W���f�[�^
	std::map<std::string, Origin>origin;
};
