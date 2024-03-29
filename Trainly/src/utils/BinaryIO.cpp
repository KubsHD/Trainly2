#include "BinaryIO.h"

void BinaryIO::WriteString(std::ofstream& bin, const std::string& value)
{
	bin.write(value.data(), value.length() + 1);
}

void BinaryIO::WriteShort(std::ofstream& bin, int16_t value)
{
	bin.put(static_cast<uint8_t>(value & 0xff));
	bin.put(static_cast<uint8_t>((value >> 8) & 0xff));
}

//void BinaryIO::WriteByte(std::ofstream& bin, char value)
//{
//	bin.write(&value, 1);
//}

void BinaryIO::WriteInt(std::ofstream& bin, int value)
{
	bin.write((char*)&value, 4);
}

std::string BinaryIO::ReadString(std::ifstream& bin)
{
	char data[256];
	bin.read(data, 1);
	char* chr = data;
	while (*chr != '\0')
		bin.read(++chr, 1);
	return data;
}

int16_t BinaryIO::ReadShort(std::ifstream& bin)
{
	int16_t value;
	bin.read(reinterpret_cast<char*>(&value), 2);
	return value;
}

int BinaryIO::ReadInt(std::ifstream& bin)
{
	int value;
	bin.read(reinterpret_cast<char*>(&value), 4);
	return value;
}
//
//template <class T>
//void BinaryIO::WriteVector(std::ofstream& bin, const std::vector<T>& value)
//{
//	WriteInt(bin, (int)value.size());
//	//std::cout << value.size() * sizeof(T) << std::endl;
//	bin.write((char*)&value[0], value.size() * sizeof(T));
//}
//
//template <class T>
//std::vector<T> BinaryIO::ReadVector(std::ifstream& bin)
//{
//	auto size = ReadInt(bin);
//	std::cout << size << std::endl;
//	std::vector<T> data;
//	data.resize(size);
//	bin.read((char*)&data[0], size * sizeof(T));
//
//	return data;
//}