#include "ModelCompiler.h"
#include <vector>
#include <assimp\mesh.h>
#include <cassert>
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include <iostream>
#include <fstream>
#include <filesystem>
#include "../utils/BinaryIO.h"

using namespace std;

struct VERTEX
{
	struct {
		float x;
		float y;
		float z;
	} position;

	struct {
		float u;
		float v;
	} uv;

	struct {
		float x;
		float y;
		float z;
	} normal;
};

struct MESH
{
	uint64_t vertex_count;
	uint64_t index_count;

	std::vector<uint32_t> indices;
	std::vector<VERTEX> vertices;

	std::string texture_name;

	void Write(std::ofstream& bin)
	{


		BinaryIO::WriteInt(bin, vertex_count);
		BinaryIO::WriteInt(bin, index_count);

		BinaryIO::WriteString(bin, texture_name);

		BinaryIO::WriteVector<VERTEX>(bin, vertices);
		BinaryIO::WriteVector<uint32_t>(bin, indices);

	}

	void Read(std::ifstream& bin)
	{
		vertex_count = BinaryIO::ReadInt(bin);
		index_count = BinaryIO::ReadInt(bin);

		texture_name = BinaryIO::ReadString(bin);

		vertices = BinaryIO::ReadVector<VERTEX>(bin);
		indices = BinaryIO::ReadVector<uint32_t>(bin);
	}
};

struct MODEL
{
	std::string SIGNATURE = "SMDL";
	uint32_t VERSION = 1;

	std::string name;

	uint32_t meshCount;
	std::vector<MESH> meshes;
};

void ModelCompiler::Process(std::string path, std::string targetPath)
{
	MODEL md;

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_FlipUVs);

	std::filesystem::path target(targetPath);
	target.replace_extension(".smo");

	//auto targetInfo = target.replace_extension(".modelinfo");
	//std::ofstream outinfo(targetInfo);


	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}

	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		std::vector<VERTEX> vertices;
		std::vector<uint32_t> index_data_array;

		const auto mesh = scene->mMeshes[i];


		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			VERTEX v;
			v.position = {
				.x = mesh->mVertices[i].x,
				.y = mesh->mVertices[i].y,
				.z = mesh->mVertices[i].z
			};

			if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
			{
				v.uv.u = mesh->mTextureCoords[0][i].x;
				v.uv.v = mesh->mTextureCoords[0][i].y;
			}
			else
			{
				v.uv.u = 0.0f;
				v.uv.v = 0.0f;
			}

			v.normal = {
				.x = mesh->mNormals[i].x,
				.y = mesh->mNormals[i].y,
				.z = mesh->mNormals[i].z
			};

			vertices.push_back(v);
		}

		index_data_array.reserve(static_cast<std::vector<uint32_t, std::allocator<uint32_t>>::size_type>(mesh->mNumFaces) * 3);
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];

			assert(face.mNumIndices == 3);

			index_data_array.push_back(face.mIndices[0]);
			index_data_array.push_back(face.mIndices[1]);
			index_data_array.push_back(face.mIndices[2]);

		}


		//std::cout << index_data_array.size() << std::endl;

		// material 

		const auto& mat = scene->mMaterials[mesh->mMaterialIndex];
		aiString path;

		aiString prop;
		mat->Get(AI_MATKEY_NAME, prop);

		mat->GetTexture(aiTextureType::aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL);

		std::string texName;

		if (path.C_Str() != "")
			texName = std::filesystem::path(path.C_Str()).filename().stem().string();
		else
			texName = "";

		MESH msh = {
			.vertex_count = vertices.size(),
			.index_count = index_data_array.size(),
			.indices = index_data_array,
			.vertices = vertices,
			.texture_name = texName
		};
		
		//outinfo.write((char*)vertices.size(), sizeof(size_t));
		//outinfo.write((char*)index_data_array.size(), sizeof(size_t));

		md.meshes.push_back(msh);
	}

	// prepare model



	std::ofstream out(target, std::ios::binary);

	md.meshCount = md.meshes.size();
	md.name = target.filename().stem().string();

	BinaryIO::WriteString(out, md.SIGNATURE);
	BinaryIO::WriteShort(out, md.VERSION);
	BinaryIO::WriteShort(out, md.meshCount);
	BinaryIO::WriteString(out, md.name);


	for (auto msh : md.meshes)
	{
		msh.Write(out);
	}
	

	out.close();

	// write stats

	//outinfo.close();


}

void ModelCompiler::Decompile(std::string path, std::string targetPath)
{
	std::ifstream file(path, std::ios::binary);

	MODEL md;

	md.SIGNATURE = BinaryIO::ReadString(file);
	md.VERSION = BinaryIO::ReadShort(file);
	md.meshCount = BinaryIO::ReadShort(file);
	md.name = BinaryIO::ReadString(file);

	std::cout << md.SIGNATURE << " " << md.VERSION << std::endl;

	for (int i = 0; i < md.meshCount; i++)
	{
		MESH ms;
		ms.Read(file);

		md.meshes.push_back(ms);
	}


	std::cout << md.name << std::endl;

	for (int i = 0; i < md.meshCount; i++)
	{
		auto mesh = md.meshes[i];

		std::cout << "Indices: " << mesh.indices.size() << " Vertices:" << mesh.vertices.size() << std::endl;
	}

}
