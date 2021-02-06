#pragma once

#include "buffer.h"

class IndexBuffer : public Buffer
{
public:
	IndexBuffer() { }

	void Create(ID3D11Device* device, uint32_t* data, UINT indicesCount)
	{
		D3D11_BUFFER_DESC index_buffer_desc = {
			.ByteWidth = sizeof(uint32_t) * indicesCount,
			.Usage = D3D11_USAGE_DEFAULT,
			.BindFlags = D3D11_BIND_INDEX_BUFFER,
			.CPUAccessFlags = 0,
			.MiscFlags = 0,
			.StructureByteStride = sizeof(uint32_t)
		};

		D3D11_SUBRESOURCE_DATA sr_data = {
			.pSysMem = data
		};

		m_indexCount = indicesCount;

		DX::ThrowIfFailed(device->CreateBuffer(&index_buffer_desc, &sr_data, m_buffer.GetAddressOf()));
	};

	int Count() { return m_indexCount; }

private:
	UINT m_indexCount = 0;
};

