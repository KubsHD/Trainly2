#include "TextureCompiler.h"

#include <DirectXTex.h>
#include "../utils/StringConverter.h"
#include <filesystem>
#include <fstream>

using namespace DirectX;

void TextureCompiler::Process(std::string path, std::string targetPath)
{
	TexMetadata info;
	auto image = std::make_unique<ScratchImage>();

	CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	LoadFromWICFile(StringConverter::StringToWide(path).c_str(), WIC_FLAGS_NONE, &info, *image, nullptr);

	DirectX::ScratchImage img;

	std::filesystem::path finalPath = targetPath;

	finalPath.replace_extension(".stx");

	//Compress(image->GetImages(), image->GetImageCount(), image->GetMetadata(), DXGI_FORMAT_, TEX_COMPRESS_FLAGS::TEX_COMPRESS_PARALLEL, TEX_THRESHOLD_DEFAULT, img);

	SaveToDDSFile(*image->GetImages(), DDS_FLAGS_NONE, StringConverter::StringToWide(finalPath.string()).c_str());
}

void TextureCompiler::Decompile(std::string path, std::string targetPath)
{

}
