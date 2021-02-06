#pragma once
#include "BaseAssetCompiler.h"



class ModelCompiler :
    public BaseAssetCompiler
{

    // .smo

public: 
    ModelCompiler() { this->Name = "Model Compiler"; }

    void Process(std::string path, std::string targetPath) override;
    void Decompile(std::string path, std::string targetPath = "") override;
};

