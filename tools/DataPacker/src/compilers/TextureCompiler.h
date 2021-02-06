#pragma once
#include "BaseAssetCompiler.h"

class TextureCompiler : public BaseAssetCompiler
{

	// .stx

public:
	TextureCompiler() { this->Name = "Texture Compiler"; }

	void Process(std::string path, std::string targetPath) override;
	void Decompile(std::string path, std::string targetPath = "") override;

};

