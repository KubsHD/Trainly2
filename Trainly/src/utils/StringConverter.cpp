#include "StringConverter.h"

std::wstring StringConverter::StringToWide(std::string str)
{
	std::wstring wide_string(str.begin(), str.end());
	return wide_string;
}

std::vector<std::string> StringConverter::ReadAllLines(std::string path)
{
	std::ifstream file(path);
	std::string str;
	std::vector<std::string> file_contents;
	while (std::getline(file, str))
	{
		file_contents.push_back(str);
	}

	return file_contents;
}
