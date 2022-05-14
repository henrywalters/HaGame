#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include <string>
#include <charconv>
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

			ConfigFile() {}

			ConfigFile(hagame::utils::File* file)
			{
				m_parsed = parseConfigFile(file);
			}

			bool hasKey(std::string section, std::string key) {
				return m_parsed.find(section) != m_parsed.end() && m_parsed[section].find(key) != m_parsed[section].end();
			}

			void assertSection(std::string section) {
				if (m_parsed.find(section) == m_parsed.end()) {
					std::cout << "Section: " << section << " does not exist\n";
					// throw new std::exception("Section does not exist");
				}
			}

			void assertKey(std::string section, std::string key) {
				
				assertSection(section);

				if (m_parsed[section].find(key) == m_parsed[section].end()) {
					std::cout << "Key: " << key << " in section: " << section << " does not exist\n";
					//throw new std::exception("Key does not exist");
				}
			}

			template <class T>
			T getValue(String section, String key) {
				auto value = T{};
				auto raw = getRaw(section, key);
				auto [ptr, err] = std::from_chars(raw.data(), raw.data() + raw.size(), value);
				if (err != std::errc{}) {
					std::cout << "Raw = " << raw << "\n";
					//throw new std::exception("Failed to convert line to numeric value");
				}
				return value;
			}

			std::vector<std::string> getSections() {
				std::vector<std::string> sections;
				for (auto& [section, data] : m_parsed) {
					sections.push_back(section);
				}
				return sections;
			}

			void addSection(String section) {
				m_parsed.insert(std::make_pair(section, std::unordered_map<std::string, std::string>()));
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

			template <class T, size_t N>
			void getArray(std::string section, std::string key, T (&arr)[N]) {
				assertKey(section, key);
				std::array<T, N> out;
				auto line = getRaw(section, key);
				auto parts = stringSplit(line, ',');
				for (int i = 0; i < N; i++) {
					auto value = T{};
					auto [ptr, err] = std::from_chars(parts[i].data(), parts[i].data() + parts[i].size(), value);
					if (err != std::errc{}) {
						throw new std::exception("Failed to convert line to numeric value");
					}
					arr[i] = value;
				}
			}

			template <class T>
			void setValue(std::string section, std::string key, T value) {
				m_parsed[section].insert(std::make_pair(key, std::to_string(value)));
			}

			template <class T, size_t N>
			void setArray(std::string section, std::string key, T arr[N]) {
				String raw = "";
				for (int i = 0; i < N; i++) {
					raw += std::to_string(arr[i]);
					if (i < N - 1) {
						raw += ',';
					}
				}
				m_parsed[section].insert(std::make_pair(key, raw));
			}

			void writeToFile(hagame::utils::File* file) {
				auto lines = std::vector<std::string>();
				for (auto& [sectionName, section] : m_parsed) {
					lines.push_back("[" + sectionName + "]\n");
					for (auto& [keyName, rawValue] : section) {
						lines.push_back(keyName + "=" + rawValue + "\n");
					}
					lines.push_back("\n");
				}
				file->writeLines(lines);
			}

		private:
			ParsedConfigFile m_parsed;
		};


		class Configurable {
		public:
			virtual void loadFromConfig(ConfigFile config) = 0;
			virtual ConfigFile saveToConfig() = 0;
		protected:
		};
	}
}


#endif
