#pragma once

#include "../../stdafx.h"

class VertexShader
{
public:
	void Create(ID3D11Device* device, std::vector<D3D11_INPUT_ELEMENT_DESC> inputElement, const std::wstring shaderPath);

	ID3D11VertexShader* Get() { return m_shader.Get(); }
	ID3DBlob* GetBlob() { return m_blob.Get(); }
	ID3D11InputLayout* GetLayout() { return m_layout.Get(); }
private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_shader;
	Microsoft::WRL::ComPtr<ID3DBlob> m_blob;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_layout;
};

