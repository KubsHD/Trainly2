#pragma once

#include "../../stdafx.h"

class PixelShader
{
public:
	void Create(ID3D11Device* device, const std::wstring shaderPath);

	ID3D11PixelShader* Get() { return m_shader.Get(); }
	ID3DBlob* GetBlob() { return m_blob.Get(); }
private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_shader;
	Microsoft::WRL::ComPtr<ID3DBlob> m_blob;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_layout;
};

