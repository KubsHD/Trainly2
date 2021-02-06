#include "VertexShader.h"

std::wstring s2ws(const std::string& s)
{
    int len;
    int slength = (int)s.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
    std::wstring r(buf);
    delete[] buf;
    return r;
}


void VertexShader::Create(ID3D11Device* device, std::vector<D3D11_INPUT_ELEMENT_DESC> inputElement, const std::wstring shaderPath)
{
	// load file
	DX::ThrowIfFailed(D3DReadFileToBlob(shaderPath.c_str(), m_blob.GetAddressOf()));
	DX::ThrowIfFailed(device->CreateVertexShader(m_blob->GetBufferPointer(), m_blob->GetBufferSize(), NULL, m_shader.GetAddressOf()));

    DX::ThrowIfFailed(device->CreateInputLayout(&inputElement[0], inputElement.size(), m_blob->GetBufferPointer(), m_blob->GetBufferSize(), m_layout.GetAddressOf()));
}
