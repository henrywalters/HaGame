#include "File.h"

String hagame::utils::File::read()
{
	std::ifstream stream(dir + "/" + name);
	String str = std::string(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
	stream.close();
	return str;
}

Array<String> hagame::utils::File::readLines()
{
	return stringSplit(read(), '\n');
}

void hagame::utils::File::write(String lines)
{
	std::ofstream stream(dir + "/" + name, std::ios::trunc);
	stream << lines;
	stream.close();
}

void hagame::utils::File::writeLines(Array<String> lines)
{
	std::ofstream stream(dir + "/" + name, std::ios::trunc);
	for (auto line : lines) {
		stream << line;
	}
	stream.close();
}

void hagame::utils::File::append(String lines)
{
	std::ofstream stream(dir + "/" + name, std::ios::app);
	stream << lines;
	stream.close();
}

void hagame::utils::File::appendLines(Array<String> lines)
{
	std::ofstream stream(dir + "/" + name, std::ios::app);
	for (auto line : lines) {
		stream << line;
	}
	stream.close();
}
