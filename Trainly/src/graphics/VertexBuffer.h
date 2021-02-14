#pragma once
#include "buffer.h"

template<class T>
class VertexBuffer : public Buffer
{
public:
	VertexBuffer() { };
	void Create(ID3D11Device* device, T* data, UINT vertexCount)
	{
		D3D11_BUFFER_DESC vertex_buffer_desc = {
		.ByteWidth = (UINT)(sizeof(T) * vertexCount),
		.Usage = D3D11_USAGE_DEFAULT,
		.BindFlags = D3D11_BIND_VERTEX_BUFFER,
		.CPUAccessFlags = 0u,
		.MiscFlags = 0u,
		.StructureByteStride = sizeof(T),
		};

		m_vertexCount = vertexCount;

		D3D11_SUBRESOURCE_DATA sr_data;

		sr_data = {
			.pSysMem = data
		};

		DX::ThrowIfFailed(device->CreateBuffer(&vertex_buffer_desc, &sr_data, m_buffer.GetAddressOf()));
	};

	UINT Stride() { return m_stride; }

	UINT* pStride() { return &m_stride; }
	UINT* pOffset() { return &m_offset; }


	UINT Count() { return m_vertexCount; }

private:
	UINT m_offset = 0;
	UINT m_stride = sizeof(T);
	UINT m_vertexCount = 0;
};
