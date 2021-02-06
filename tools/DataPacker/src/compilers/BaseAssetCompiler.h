#pragma once
#include <string>

class BaseAssetCompiler
{
public:

    std::string Name = "Base Compiler";

    virtual ~BaseAssetCompiler() {}
    virtual void Process(std::string path, std::string targetPath) = 0;

    virtual void Decompile(std::string path, std::string targetPath = "") = 0;
};