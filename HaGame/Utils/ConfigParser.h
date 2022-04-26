#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include <string>
#include <unordered_map>
#include "./File.h"

namespace hagame {
	namespace utils {

		using ParsedConfigFile = std::unordered_map<std::string, std::unordered_map<std::string, std::string>>;


		ParsedConfigFile parseConfigFile(hagame::utils::File* file) {
			ParsedConfigFile parsed;
			std::string sectionName;
			for (auto line : file->readLines()) {
				line = stringTrim(line);
				if (line[0] == '[' && line[line.size() - 1] == ']') {
					sectionName = stringSlice(line, 1, line.size() - 1);
					if (parsed.find(sectionName) == parsed.end()) {
						parsed.insert(std::make_pair(sectionName, std::unordered_map<std::string, std::string>()));
					}
				}
				else if (line != "") {

					auto parts = stringSplit(line, '=');
					if (parts.size() != 2) {
						std::cout << "Invalid config line: " << line << "\n";
						throw new std::exception("Invalid configuration file");
					}
					else {
						if (parsed[sectionName].find(parts[0]) != parsed[sectionName].end()) {
							std::cout << "Duplicate key found in section: " << sectionName << ", " << parts[0] << "\n";
							throw new std::exception("INvalid configuration file");
						}

						parsed[sectionName].insert(std::make_pair(parts[0], parts[1]));
					}
				}
			}
			return parsed;
		}


		class ConfigFile {
		public:

			ConfigFile(hagame::utils::File* file) {
				m_parsed = parseConfigFile(file);
			}

			void assertKey(std::string section, std::string key) {
				if (m_parsed.find(section) == m_parsed.end()) {
					std::cout << "Section: " << section << " does not exist\n";
					throw new std::exception("Section does not exist");
				}

				if (m_parsed[section].find(key) == m_parsed[section].end()) {
					std::cout << "Key: " << key << " in section: " << section << " does not exist\n";
					throw new std::exception("Key does not exist");
				}
			}

			std::vector<std::string> getSections() {
				std::vector<std::string> sections;
				for (auto& [section, data] : m_parsed) {
					sections.push_back(section);
				}
				return sections;
			}

			std::string getRaw(std::string section, std::string key) {
				assertKey(section, key);
				return m_parsed[section][key];
			}

			int getInt(std::string section, std::string key) {
				return std::stoi(getRaw(section, key));
			}

			float getFloat(std::string section, std::string key) {
				return std::stof(getRaw(section, key));
			}

			double getDouble(std::string section, std::string key) {
				return std::stod(getRaw(section, key));
			}

			bool getBool(std::string section, std::string key) {
				return std::stoi(getRaw(section, key)) == 1 ? true : false;
			}

		private:
			ParsedConfigFile m_parsed;
		};

	}
}


#endif
