#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include <string>
#include <charconv>
#include <unordered_map>
#include "./File.h"
#include "./Serializable.h"

namespace hagame {
	namespace utils {

		/*
		
			The ConfigFile and MultiConfigFile classes provide an interface for saving / loading 
			configuration data from files (or other sources) as well as accessing type safe variables.

			The philosophy behind config files (rather than using JSON / xml) is to encourage
			storing the simplest data types possible for any given configurable object.

			A config section consists of "Sections". Each section has a set of key/value pairs.

			A section looks like this:

			[SectionName]
			key1=1.000000,2.000000,3.000000
			key2={1,2}{2,3}
			key3=Hello World

			Arrays and Array of Arrays are the only aggregate data types supported at this time.

			A Multi Config consists of several "Pages" of configuration.

			A page looks like this:

			~Page 1~

			[SectionA]
			key1=Hello

			[SectionB]
			key1=World
			
		*/

		using ParsedConfigFile = Map<String, Map<String, String>>;
		using ParsedMultiConfigFile = Map<String, ParsedConfigFile>;

		ParsedConfigFile parseConfigString(Array<String> configLines) {
			ParsedConfigFile parsed;
			std::string sectionName;
			for (auto line : configLines) {
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

		ParsedMultiConfigFile parseMultiConfigString(Array<String> configLines) {
			ParsedMultiConfigFile config;
			Map<String, Array<String>> pages;

			String pageName = "";
			Array<String> page;

			for (auto line : configLines) {
				line = stringTrim(line);
				if (line[0] == '~' && line[line.size() - 1] == '~') {
					if (pageName != "") {
						pages.insert(std::make_pair(pageName, page));
					}
					page.clear();
					pageName = stringSlice(line, 1, line.size() - 1);
					if (pageName == "") {
						throw new std::exception("Page name must be non-empty");
					}

				}
				else if (line != "") {
					if (pageName == "") {
						throw new std::exception("Encountered section before page was declared");
					}
					page.push_back(line);
				}
			}

			if (pageName != "") {
				pages.insert(std::make_pair(pageName, page));
			}

			for (auto& [name, page] : pages) {
				config.insert(std::make_pair(name, parseConfigString(page)));
			}

			return config;
		}

		ParsedConfigFile parseConfigFile(hagame::utils::File* file) {
			return parseConfigString(file->readLines());
		}

		ParsedMultiConfigFile parseMultiConfigFile(hagame::utils::File* file) {
			return parseMultiConfigString(file->readLines());
		}


		class ConfigFile {
		public:

			ConfigFile() {}

			ConfigFile(hagame::utils::File* file)
			{
				m_parsed = parseConfigFile(file);
			}

			ConfigFile(ParsedConfigFile parsed) {
				m_parsed = parsed;
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

			template <class T, size_t N>
			void setListOfArrays(std::string section, std::string key, std::vector<std::array<T, N>> arrs) {
				String raw = "";
				for (auto arr : arrs) {
					String rawValue = "";
					for (int i = 0; i < N; i++) {
						rawValue += std::to_string(arr[i]);
						if (i < N - 1) {
							rawValue += ',';
						}
					}
					raw += "{" + rawValue + "}";
				}
				m_parsed[section].insert(std::make_pair(key, raw));
			}

			template <class T, size_t N>
			std::vector<std::array<T, N>> getListOfArrays(std::string section, std::string key) {
				auto out = std::vector<std::array<T, N>>();
				auto chunks = stringPartition(getRaw(section, key), '{', '}');

				for (auto chunk : chunks) {
					auto parts = stringSplit(chunk, ',');
					auto arr = std::array<T, N>();
					for (int i = 0; i < N; i++) {
						auto value = T{};
						auto [ptr, err] = std::from_chars(parts[i].data(), parts[i].data() + parts[i].size(), value);
						if (err != std::errc{}) {
							throw new std::exception("Failed to convert line to numeric value");
						}
						arr[i] = value;
					}
					out.push_back(arr);
				}

				return out;
			}

			void forEach(String section, std::function<void(String, String)> lambda) {
				for (auto& [key, value] : m_parsed[section]) {
					lambda(key, value);
				}
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

		class MultiConfigFile {
		public:
			MultiConfigFile() {}

			MultiConfigFile(hagame::utils::File* file) {
				auto parsed = parseMultiConfigFile(file);
				for (auto& [name, page] : parsed) {
					m_configs.insert(std::make_pair(name, ConfigFile(page)));
				}
			}

			void addPage(String name) {
				m_configs.insert(std::make_pair(name, ConfigFile()));
			}

			ConfigFile* getPage(String name) {
				return &m_configs[name];
			}

			void writeToFile(hagame::utils::File* file) {
				auto lines = std::vector<std::string>();
				for (auto& [name, page] : m_configs) {
					lines.push_back("~" + name + "~\n");
					for (auto section : page.getSections()) {
						lines.push_back("[" + section + "]\n");
						page.forEach(section, [&lines](String key, String value) {
							lines.push_back(key + "=" + value + "\n");
						});
						lines.push_back("\n");
					}
					lines.push_back("\n");
				}

				file->writeLines(lines);
			}

		private:
			Map<String, ConfigFile> m_configs;
		};


		// If a class inherits from Configurable, it is able to be saved and loaded from a .config file
		class Configurable {
		public:
			virtual void loadFromConfig(ConfigFile config) = 0;
			virtual ConfigFile saveToConfig() = 0;
		protected:
		};
	}
}


#endif
