#pragma once
#include "../stdafx.h"

class BinaryIO {
public:
	static void WriteString(std::ofstream& bin, const std::string& value);
	static void WriteShort(std::ofstream& bin, int16_t value);
	static	void WriteInt(std::ofstream& bin, int value);




	static	std::string ReadString(std::ifstream& bin);
	static	int16_t ReadShort(std::ifstream& bin);
	static	int ReadInt(std::ifstream& bin);


	template <class T>
	static std::vector<T> ReadVector(std::ifstream& bin)
	{
		auto size = ReadInt(bin);
		//std::cout << size << std::endl;
		std::vector<T> data;
		data.resize(size);
		bin.read((char*)&data[0], size * sizeof(T));

		return data;
	}

	template <class T>
	static void WriteVector(std::ofstream& bin, const std::vector<T>& value)
	{
		WriteInt(bin, (int)value.size());
		//std::cout << value.size() * sizeof(T) << std::endl;
		bin.write((char*)&value[0], value.size() * sizeof(T));
	}

};
