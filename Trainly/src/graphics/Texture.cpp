#include "../stdafx.h"


#include "texture.h"

void Texture::Create(ID3D11Device* device, std::string path)
{
	DX::ThrowIfFailed(DirectX::CreateDDSTextureFromFile(
		device,
		StringConverter::StringToWide(path).c_str(),
		m_texture.GetAddressOf(),
		m_shaderResource.GetAddressOf()
	));
}
