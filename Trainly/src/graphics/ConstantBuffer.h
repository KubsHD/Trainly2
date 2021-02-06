#pragma once

#include "buffer.h"

template<class T>
class ConstantBuffer : public Buffer
{
public:
	ConstantBuffer() {};

	void Create(ID3D11Device* device, T* data)
	{
		D3D11_BUFFER_DESC constant_buffer_desc = {
		.ByteWidth = sizeof(T),
		.Usage = D3D11_USAGE_DYNAMIC,
		.BindFlags = D3D11_BIND_CONSTANT_BUFFER,
		.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE,
		.MiscFlags = 0u,
		.StructureByteStride = 0,
		};

		D3D11_SUBRESOURCE_DATA sr_data = {
			.pSysMem = data
		};

		DX::ThrowIfFailed(device->CreateBuffer(&constant_buffer_desc, &sr_data, m_buffer.GetAddressOf()));
	}

	void Create(ID3D11Device* device)
	{
		D3D11_BUFFER_DESC constant_buffer_desc = {
		.ByteWidth = sizeof(T),
		.Usage = D3D11_USAGE_DYNAMIC,
		.BindFlags = D3D11_BIND_CONSTANT_BUFFER,
		.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE,
		.MiscFlags = 0u,
		.StructureByteStride = 0,
		};

		DX::ThrowIfFailed(device->CreateBuffer(&constant_buffer_desc, nullptr, m_buffer.GetAddressOf()));
	}

	void UpdateBuffer(ID3D11DeviceContext* context, T* data)
	{
		D3D11_MAPPED_SUBRESOURCE sub;

		DX::ThrowIfFailed(context->Map(m_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &sub));
		CopyMemory(sub.pData, data, sizeof(T));
		context->Unmap(m_buffer.Get(), 0);
	}
};

