#pragma once

#include "../stdafx.h"

#include <DDSTextureLoader.h>

class Texture
{
public:
	Texture() { };

	std::string Name;

	void Create(ID3D11Device* device, std::string path);

	ID3D11ShaderResourceView** pGetShaderResource() { return m_shaderResource.GetAddressOf(); }
private:
	Microsoft::WRL::ComPtr<ID3D11Resource> m_texture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_shaderResource;
};

