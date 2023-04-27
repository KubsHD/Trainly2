#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <d3d11.h>       // D3D interface
#include <dxgi.h>        // DirectX driver interface
#include <d3dcompiler.h> // shader compiler
#include <iostream>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <vector>
#include <map>

#include <cereal/types/map.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/complex.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/specialize.hpp>
#include <cereal/cereal.hpp>

#include "utils/MD5.h"

#include "compilers/BaseAssetCompiler.h"
#include "compilers/TextureCompiler.h"
#include "compilers/ModelCompiler.h"
#include "compilers/ShaderCompiler.h"

static std::vector<char> ReadAllBytes(std::string filename)
{
	std::ifstream ifs(filename, std::ios::binary | std::ios::ate);
	std::ifstream::pos_type pos = ifs.tellg();

	std::vector<char>  result(pos);

	ifs.seekg(0, std::ios::beg);
	ifs.read(&result[0], pos);

	return result;
}

namespace fs = std::filesystem;

void copy_content(std::string srcDir, std::string dst)
{
	std::cout << "Writing files from source directory located in: " << srcDir << " to output directory: " << dst << std::endl;

	std::filesystem::path baseDir(srcDir);
	std::filesystem::path targetDir(dst);
	fs::create_directory(targetDir);

	for (fs::recursive_directory_iterator i(srcDir), end; i != end; ++i)
	{
		if (!i->is_directory())
		{
			auto path = i->path();

			auto fileName = i->path().stem();
			auto absoluteFilePath = i->path().parent_path();
			auto fileExtension = i->path().extension();

			auto pathInContentDirectory = i->path().parent_path().lexically_relative(baseDir);

			auto aboslutePathInTargetDirectory = targetDir / pathInContentDirectory;

			auto abosluteFilePathInTargetDirectory = aboslutePathInTargetDirectory / std::filesystem::path(fileName.string() + fileExtension.string());
			
			std::cout << "-----------------" << std::endl;
			std::cout << fileName << std::endl;
			std::cout << absoluteFilePath << std::endl;
			std::cout << fileExtension << std::endl;
			std::cout << pathInContentDirectory << std::endl;
			std::cout << aboslutePathInTargetDirectory << std::endl;

			fs::create_directories(aboslutePathInTargetDirectory);
			fs::copy_file(path, abosluteFilePathInTargetDirectory, std::filesystem::copy_options::overwrite_existing);
		}
	}
}

struct hash_entry
{
	std::string path;
	std::string md5;

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(CEREAL_NVP(path), CEREAL_NVP(md5));
	}
};

void build_content(std::string srcDir, std::string dst)
{
	std::map<std::string, BaseAssetCompiler*> compilers =
	{
		{ ".png", new TextureCompiler },
		{ ".tif", new TextureCompiler },
		{ ".hlsl", new ShaderCompiler },
		{ ".fbx", new ModelCompiler },
		{ ".glb", new ModelCompiler },
		{ ".gltf", new ModelCompiler },
	};

	std::vector<hash_entry> hash_entries;

	auto hashPath = dst + "\\cache.hash";

	if (fs::exists(hashPath))
	{
		std::ifstream str(hashPath);
		cereal::JSONInputArchive archive(str);
		archive(hash_entries);
	}


	std::cout << "Writing files from source directory located in: " << srcDir << " to output directory: " << dst << std::endl;

	std::filesystem::path baseDir(srcDir);
	std::filesystem::path targetDir(dst);
	fs::create_directory(targetDir);

	std::fstream cacheFile(md5(srcDir));

	for (fs::recursive_directory_iterator i(srcDir), end; i != end; ++i)
	{
		if (!i->is_directory())
		{
			auto path = i->path();

			auto fileName = i->path().stem();
			auto absoluteFilePath = i->path().parent_path();
			auto fileExtension = i->path().extension();

			auto pathInContentDirectory = i->path().parent_path().lexically_relative(baseDir);

			auto aboslutePathInTargetDirectory = targetDir / pathInContentDirectory;

			auto abosluteFilePathInTargetDirectory = aboslutePathInTargetDirectory / std::filesystem::path(fileName.string() + fileExtension.string());

			/*std::cout << "-----------------" << std::endl;
			std::cout << fileName << std::endl;
			std::cout << absoluteFilePath << std::endl;
			std::cout << fileExtension << std::endl;
			std::cout << pathInContentDirectory << std::endl;
			std::cout << aboslutePathInTargetDirectory << std::endl;*/


			auto fileData = ReadAllBytes(path.string());
			auto fileMD5 = md5(std::string(fileData.begin(), fileData.end()));

			bool fileChanged = true;
			bool fileInHash = false;

			for (auto entry : hash_entries)
			{
				if (entry.path == abosluteFilePathInTargetDirectory.string())
				{
					if (entry.md5 == fileMD5)
						fileChanged = false;
					else
						entry.md5 == fileMD5;

					fileInHash = true;
				}
			}

			if (!fileInHash)
				hash_entries.push_back({ abosluteFilePathInTargetDirectory.string(), fileMD5 });

			if (!fileChanged)
				continue;


			fs::create_directories(aboslutePathInTargetDirectory);


			if (compilers.find(fileExtension.string()) != compilers.end())
			{
				std::cout << "[INFO] " << "Processing: " << fileName.string() + fileExtension.string() << " with compiler: " << compilers[fileExtension.string()]->Name << std::endl;
				compilers[fileExtension.string()]->Process(path.string(), abosluteFilePathInTargetDirectory.string());
			}
			else
			{
				std::cout << "[WARNING] " << "No suitable compiler found for: " << fileName.string() + fileExtension.string() << ", copying!" << std::endl;
				fs::copy_file(path, abosluteFilePathInTargetDirectory, std::filesystem::copy_options::overwrite_existing);
			}
		}
	}


	std::ofstream str(hashPath);
	cereal::JSONOutputArchive archive(str);
	archive(hash_entries);

}

void decompile(std::string path)
{
	std::map<std::string, BaseAssetCompiler*> compilers =
	{
		{ ".stx", new TextureCompiler },
		{ ".scsh", new ShaderCompiler },
		{ ".smo", new ModelCompiler }
	};

	std::filesystem::path i(path);

	auto fileName = i.stem();
	auto absoluteFilePath = i.parent_path();
	auto fileExtension = i.extension();

	compilers[fileExtension.string()]->Decompile(path);
}

int main(int argc, char* argv[])
{

	std::cout << "HELP - packer" << std::endl;

	std::cout << " w - write " << std::endl;
	std::cout << " u - unpack" << std::endl;

	std::cout << " packer.exe buildContent [srcdir] [outdir]" << std::endl;

	std::cout << "Command-line argument count: " << argc << " \n";
	std::cout << "Arguments:\n";
	for (int i = 0; i < argc; i++)
		std::cout << "  argv[" << i << "]   "
		<< argv[i] << "\n";

	if (argc > 1)
	{
		std::string mode = argv[1];
		std::string arg1 = argv[2];
		std::string arg2 = "";

		if (argv[3] != NULL)
			arg2 = argv[3];

		if (mode == "buildContent")
			build_content(arg1, arg2);

		if (mode == "decompile")
			decompile(arg1);

		if (mode == "copyContent")
			copy_content(arg1, arg2);
	}


	return 0;
}