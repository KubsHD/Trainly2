#include "../stdafx.h"


#include "mesh.h"

void Mesh::Create(ID3D11Device* device, std::vector<Vertex> vertices, std::vector<uint32_t> indices, int matId)
{
	m_indexBuffer.Create(device, indices.data(), indices.size());
	m_vertexBuffer.Create(device, &vertices[0], vertices.size());

	m_materialId = matId;
}
