#pragma once
#include <string>

class StringConverter
{
public:
	static std::wstring StringToWide(std::string str);
	static std::vector<std::string> ReadAllLines(std::string path);
};