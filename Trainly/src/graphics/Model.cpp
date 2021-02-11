#include "Model.h"

#include <filesystem>

using namespace DirectX::SimpleMath;

void Model::Create(ID3D11Device* device, std::string path)
{
	
	std::ifstream file(path, std::ios::binary);


	if (!file.is_open())
	{
		std::string error = "Error file: " + path + "not found!";

		throw std::exception(error.c_str());
	}

	MODEL md;

	md.SIGNATURE = BinaryIO::ReadString(file);
	md.VERSION = BinaryIO::ReadShort(file);
	md.meshCount = BinaryIO::ReadShort(file);
	md.name = BinaryIO::ReadString(file);

	this->Name = md.name;

	//std::cout << md.SIGNATURE << " " << md.VERSION << std::endl;

	for (int i = 0; i < md.meshCount; i++)
	{
		MESH ms;
		ms.Read(file);
		md.meshes.push_back(ms);

		auto msh = Mesh();

		msh.Create(device, ms.vertices, ms.indices);

		m_meshes.push_back(msh);

		if (ms.texture_name != "")
		{
			auto tex = Texture();

			tex.Name = ms.texture_name;

			auto texturePath = std::filesystem::path(path).parent_path() / std::string(ms.texture_name + ".stx");

			tex.Create(device, texturePath.string());

			m_textures.push_back(tex);
		}
	}


	//std::cout << md.name << std::endl;


}



