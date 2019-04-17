#include "utility.h"

#include <cstdio>
#include <fstream>

std::unique_ptr<char[]> el::fileread(const char* filename)
{
	FILE* file = fopen(filename, "rb");
	if (file == nullptr)
		return nullptr;
	fseek(file, 0L, SEEK_END);
	long size = ftell(file);
	std::unique_ptr<char[]> data(new char[size]);
	fseek(file, 0L, SEEK_SET);
	fread(data.get(), 1, size, file);
	fclose(file);
	return std::move(data);
}

std::unique_ptr<char[]> el::fstreamread(const char* filename)
{
	std::ifstream stream;
	stream.open(filename);
	if (!stream.is_open())
		return nullptr;
	stream.ignore(std::numeric_limits<uint32_t>::max());
	auto size = static_cast<uint32_t>(stream.tellg());
	std::unique_ptr<char[]> data(new char[size]);
	stream.read(data.get(), size);
	return data;
}
