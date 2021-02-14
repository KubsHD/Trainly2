#include "PixelShader.h"

void PixelShader::Create(ID3D11Device* device, const std::wstring shaderPath)
{
	// load file
	DX::ThrowIfFailed(D3DReadFileToBlob(shaderPath.c_str(), m_blob.GetAddressOf()));
	DX::ThrowIfFailed(device->CreatePixelShader(m_blob->GetBufferPointer(), m_blob->GetBufferSize(), NULL, m_shader.GetAddressOf()));
}
