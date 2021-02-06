#pragma once
#include "BaseAssetCompiler.h"


class ShaderCompiler : public BaseAssetCompiler
{
	// .scsh

public:
	ShaderCompiler() { this->Name = "Shader Compiler"; }

	void Process(std::string path, std::string targetPath) override;
	void Decompile(std::string path, std::string targetPath = "") override;
};

