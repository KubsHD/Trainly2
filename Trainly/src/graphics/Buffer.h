#pragma once

#include "../stdafx.h"


class Buffer
{
protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
public:
	inline ID3D11Buffer* Get() const {
		return m_buffer.Get();
	};

	inline ID3D11Buffer* const* GetAddressOf() {
		return m_buffer.GetAddressOf();
	};
};