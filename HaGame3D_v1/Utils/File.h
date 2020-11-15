#ifndef FILE_H
#define FILE_H
#include <iostream>
#include <fstream>
#include <streambuf>
#include "Aliases.h"
#include "String.h"

namespace hagame {
	namespace utils {
		class File {
		public:
			String dir;
			String name;

			File(String _dir, String _name) : dir(_dir), name(_name) {}

			String read();
			Array<String> readLines();
			void write(String lines);
			void writeLines(Array<String> lines);
			void append(String lines);
			void appendLines(Array<String> lines);
		};
	}
}

#endif
