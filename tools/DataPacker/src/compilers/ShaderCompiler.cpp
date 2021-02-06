#include "ShaderCompiler.h"

#include "../utils/StringConverter.h"


#include <d3dcompiler.h>
#include <filesystem>

void ShaderCompiler::Process(std::string path, std::string targetPath)
{

	ID3DBlob* VS_BUFFER = NULL;
	ID3DBlob* errMsg = NULL;
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;

	if (path.find("vert") != std::string::npos)
	{
		D3DCompileFromFile(StringConverter::StringToWide(path).c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_4_0", dwShaderFlags, NULL, &VS_BUFFER, &errMsg);
	}

	if (path.find("pix") != std::string::npos)
	{
		D3DCompileFromFile(StringConverter::StringToWide(path).c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_4_0", dwShaderFlags, NULL, &VS_BUFFER, &errMsg);
	}

	std::filesystem::path target(targetPath);

	target.replace_extension(".scsh");

	D3DWriteBlobToFile(VS_BUFFER, StringConverter::StringToWide(target.string()).c_str(), TRUE);
}

void ShaderCompiler::Decompile(std::string path, std::string targetPath)
{
}
