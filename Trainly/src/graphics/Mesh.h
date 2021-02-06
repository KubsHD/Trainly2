#pragma once

#include "../stdafx.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"

class Mesh
{
public:
	Mesh() { };

	void Create(ID3D11Device* device, std::vector<Vertex> vertices, std::vector<uint32_t> indices, int matId = 0);

	VertexBuffer<Vertex>* GetVertexBuffer() { return &m_vertexBuffer; }
	IndexBuffer* GetIndexBuffer() { return &m_indexBuffer; }

private:
	int m_materialId;
	VertexBuffer<Vertex> m_vertexBuffer;
	IndexBuffer m_indexBuffer;
};

