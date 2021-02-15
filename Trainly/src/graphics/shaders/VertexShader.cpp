#include "VertexShader.h"

void VertexShader::Create(ID3D11Device* device, std::wstring shaderPath)
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDesc = {
{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
{ "TexCoord", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
{ "Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	// load file
	DX::ThrowIfFailed(D3DReadFileToBlob(shaderPath.c_str(), m_blob.GetAddressOf()));
	DX::ThrowIfFailed(device->CreateVertexShader(m_blob->GetBufferPointer(), m_blob->GetBufferSize(), NULL, m_shader.GetAddressOf()));
    DX::ThrowIfFailed(device->CreateInputLayout(&inputElementDesc[0], inputElementDesc.size(), m_blob->GetBufferPointer(), m_blob->GetBufferSize(), m_layout.GetAddressOf()));
}
