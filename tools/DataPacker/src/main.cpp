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

#include "compilers/BaseAssetCompiler.h"
#include "compilers/TextureCompiler.h"
#include "compilers/ModelCompiler.h"
#include "compilers/ShaderCompiler.h"

size_t strlcpy(char* d, char const* s, size_t n)
{
	return snprintf(d, n, "%s", s);
}

namespace fs = std::filesystem;

typedef unsigned char BYTE;

struct file_header
{
	char magic[6];
	int file_count;
	char type[128];
};

struct file_entry
{
	char path[256];
	char name[256];
	int size;
	int compressedSize;
	std::vector<char> data;
};

void write(std::string inputFilesDir, std::string outDir)
{
	std::cout << "Writing files from: " << inputFilesDir << " to file: " << outDir << std::endl;

	std::ofstream outFileStream;


	std::vector<file_entry> files;

	auto base_path = std::filesystem::path(inputFilesDir);

	std::string package_name = base_path.filename().string();

	for (fs::recursive_directory_iterator i(inputFilesDir), end; i != end; ++i)
	{
		if (!is_directory(i->path()) && i->path().filename().string().find(".exe") == std::string::npos && i->path().filename().string().find(".kpf") == std::string::npos)
		{
			//std::cout << i->path().filename() << "\n";

			auto root = base_path;

			root = base_path.string().substr(0, base_path.string().find_last_of("\\/"));



			auto path = i->path().lexically_relative(root).string();
			auto name = i->path().lexically_relative(base_path).string();

			file_entry fe;

			std::replace(name.begin(), name.end(), '\\', '/');
			std::replace(path.begin(), path.end(), '\\', '/');

			//name.erase(0, 2);

			std::cout << name << "\n";


			strcpy(fe.name, name.c_str());
			strcpy(fe.path, path.c_str());


			std::streampos fileSize;
			std::ifstream file(i->path().string(), std::ios::binary);

			// get its size:
			file.seekg(0, std::ios::end);
			fileSize = file.tellg();
			file.seekg(0, std::ios::beg);



			// read the data:
			std::vector<BYTE> fileData(fileSize);
			file.read((char*)&fileData[0], fileSize);

			char* compressedData = new char[fileSize];

			fe.data = reinterpret_cast<std::vector<char>&>(fileData);
			fe.size = fe.data.size();


			//strcpy(fe.data, reinterpret_cast<std::vector<const char*>&>(fileData));

			files.push_back(fe);
		}
	}

	file_header fh = { "TRPK1", static_cast<int>(files.size()) };

	strcpy(fh.type, package_name.c_str());

	outFileStream.open(outDir + "/" + package_name + ".trpk", std::ios::out | std::ios::binary);

	outFileStream.write((char*)&fh, sizeof(file_header));

	for (auto& fe : files)
	{
		outFileStream.write(fe.name, sizeof(fe.name));
		outFileStream.write(fe.path, sizeof(fe.path));
		outFileStream.write((char*)&fe.size, sizeof(fe.size));
		outFileStream.write((char*)&fe.data[0], fe.size);
	}

	outFileStream.close();
}

void write_subfolders(std::string inputFolders, std::string outDir)
{
	std::cout << "Writing files from subfolders located in: " << inputFolders << " to file: " << outDir << std::endl;

	fs::create_directory(std::filesystem::path(outDir));

	for (fs::directory_iterator i(inputFolders), end; i != end; ++i)
	{
		if (i->is_directory())
		{
			write(i->path().string(), outDir);
		}
		else
		{
			fs::copy(i->path(), outDir);
		}
	}
}

std::vector<file_entry> read(std::string filename)
{
	std::cout << filename << std::endl;

	std::ifstream file(filename, std::ios::binary);

	file_header fh;

	std::vector<file_entry> files;

	file.read((char*)&fh, sizeof(fh));

	std::cout << "Archive header: " << fh.magic << std::endl;
	std::cout << "Total file count in archive: " << fh.file_count << std::endl;

	for (int i = 0; i < fh.file_count; i++)
	{
		file_entry fe;

		file.read(fe.name, sizeof(fe.name));
		file.read(fe.path, sizeof(fe.path));
		file.read((char*)&fe.size, sizeof(fe.size));
		fe.data.resize(fe.size);
		file.read((char*)&fe.data[0], fe.size);

		files.push_back(fe);

	}

	return files;
}

file_entry get_file(char* filename, std::string assetpath)
{
	auto files = read(filename);

	for (auto file : files)
	{
		if (strcmp(file.name, assetpath.c_str()) == 0)
			return file;
	}

}

void unpack(std::string inputFile, std::string outFileDir)
{
	std::cout << "Reading files from: " << inputFile << " to dir: " << outFileDir << std::endl;


	auto files = read(inputFile);

	for (auto file : files)
	{
		fs::path path = file.path;

		fs::path base = inputFile;

		fs::create_directories(base += path.relative_path());

		std::ofstream fileOut(file.name);

		fileOut.write((char*)&file.data[0], file.size);
		fileOut.close();
	}
}

void list(std::string inputFile)
{
	std::cout << "Reading files from: " << inputFile << std::endl;


	auto files = read(inputFile);

	for (auto file : files)
	{
		std::cout << "-----------------------" << std::endl;
		std::cout << file.name << std::endl;
		std::cout << file.path << std::endl;
		std::cout << file.size << std::endl;
	}
}

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

void build_content(std::string srcDir, std::string dst)
{
	std::map<std::string, BaseAssetCompiler*> compilers =
	{
		{ ".png", new TextureCompiler },
		{ ".tif", new TextureCompiler }, 
		{ ".hlsl", new ShaderCompiler },
		{ ".fbx", new ModelCompiler }
	};

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

			/*std::cout << "-----------------" << std::endl;
			std::cout << fileName << std::endl;
			std::cout << absoluteFilePath << std::endl;
			std::cout << fileExtension << std::endl;
			std::cout << pathInContentDirectory << std::endl;
			std::cout << aboslutePathInTargetDirectory << std::endl;*/

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

	std::cout << " packer.exe w [INPUTDIR] [OUTFILE]" << std::endl;
	std::cout << " packer.exe u [INPUTFILE] [OUTPUTDIR]" << std::endl;
	std::cout << " packer.exe l [INPUTFILE]" << std::endl;
	std::cout << " packer.exe wf [INPUTFOLDERSDIR] [OUTPUTDIR]" << std::endl;

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

		if (mode == "w")
			write(arg1, arg2);

		if (mode == "wf")
			write_subfolders(arg1, arg2);

		if (mode == "u")
			unpack(arg1, arg2);

		if (mode == "l")
			list(arg1);

	}


	return 0;
}