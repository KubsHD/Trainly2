#pragma once

#include "../stdafx.h"

struct Vertex
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
	std::vector<Vertex> vertices;

	std::string texture_name;

	void Write(std::ofstream& bin)
	{
		BinaryIO::WriteInt(bin, vertex_count);
		BinaryIO::WriteInt(bin, index_count);

		BinaryIO::WriteString(bin, texture_name);

		BinaryIO::WriteVector<Vertex>(bin, vertices);
		BinaryIO::WriteVector<uint32_t>(bin, indices);

	}

	void Read(std::ifstream& bin)
	{
		vertex_count = BinaryIO::ReadInt(bin);
		index_count = BinaryIO::ReadInt(bin);

		texture_name = BinaryIO::ReadString(bin);

		vertices = BinaryIO::ReadVector<Vertex>(bin);
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