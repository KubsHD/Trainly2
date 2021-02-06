#pragma once

#include "../stdafx.h"

#include "Mesh.h"
#include "Texture.h"

class Model
{
public:

	void Create(ID3D11Device* device, std::string path);

	std::vector<Mesh>& GetMeshes() { return m_meshes; }
	std::vector<Texture>& GetTextures() { return m_textures; }

	std::string Name;

private:
	std::vector<Mesh> m_meshes;
	std::vector<Texture> m_textures;
};

