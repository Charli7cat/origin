#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <cctype>

class INIParserException : public std::runtime_error {
public:
    explicit INIParserException(const std::string& message)
        : std::runtime_error(message) {}
};

class ini_parser {
private:
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> data;

    std::string trim(const std::string& str) {
        size_t start = str.find_first_not_of(" \t\n\r\f\v");
        if (start == std::string::npos) return "";

        size_t end = str.find_last_not_of(" \t\n\r\f\v");
        return str.substr(start, end - start + 1);
    }

    std::string remove_comment(const std::string& line) {
        size_t comment_pos = line.find(';');
        if (comment_pos != std::string::npos) {
            return line.substr(0, comment_pos);
        }
        return line;
    }

    bool is_section(const std::string& line) {
        return line.length() >= 2 && line.front() == '[' && line.back() == ']';
    }

    bool is_assignment(const std::string& line) {
        return line.find('=') != std::string::npos;
    }

    std::string parse_section(const std::string& line) {
        std::string section = line.substr(1, line.length() - 2);
        return trim(section);
    }

    std::pair<std::string, std::string> parse_assignment(const std::string& line) {
        size_t eq_pos = line.find('=');
        if (eq_pos == std::string::npos) {
            throw INIParserException("Invalid assignment line: " + line);
        }

        std::string key = trim(line.substr(0, eq_pos));
        std::string value = trim(line.substr(eq_pos + 1));

        if (key.empty()) {
            throw INIParserException("Empty variable name in assignment: " + line);
        }

        return { key, value };
    }

    template<typename T>
    T convert_value(const std::string& value) const {
        std::istringstream iss(value);
        T result;

        if (iss >> result) {
            char remaining;
            if (iss >> remaining) {
                throw INIParserException("Invalid value format: " + value);
            }
            return result;
        }
        else {
            throw INIParserException("Failed to convert value: " + value);
        }
    }

    std::string convert_value<std::string>(const std::string& value) const {
        return value;
    }

public:
    ini_parser(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw INIParserException("Cannot open file: " + filename);
        }

        std::string current_section;
        std::string line;
        int line_number = 0;

        while (std::getline(file, line)) {
            line_number++;

            std::string clean_line = remove_comment(line);

            clean_line = trim(clean_line);

            if (clean_line.empty()) {
                continue;
            }

            if (is_section(clean_line)) {
                try {
                    current_section = parse_section(clean_line);
                    if (data.find(current_section) == data.end()) {
                        data[current_section] = std::unordered_map<std::string, std::string>();
                    }
                }
                catch (const INIParserException& e) {
                    throw INIParserException("Error at line " + std::to_string(line_number) + ": " + e.what());
                }
            }
            else if (is_assignment(clean_line)) {
                if (current_section.empty()) {
                    throw INIParserException("Variable outside section at line " + std::to_string(line_number));
                }

                try {
                    auto [key, value] = parse_assignment(clean_line);
                    data[current_section][key] = value;
                }
                catch (const INIParserException& e) {
                    throw INIParserException("Error at line " + std::to_string(line_number) + ": " + e.what());
                }
            }
            else {
                throw INIParserException("Unknown line format at line " + std::to_string(line_number) + ": " + line);
            }
        }

        file.close();
    }

    template<typename T>
    T get_value(const std::string& path) const {
        size_t dot_pos = path.find('.');
        if (dot_pos == std::string::npos) {
            throw INIParserException("Invalid path format. Expected 'section.key', got: " + path);
        }

        std::string section = path.substr(0, dot_pos);
        std::string key = path.substr(dot_pos + 1);

        auto section_it = data.find(section);
        if (section_it == data.end()) {
            throw INIParserException("Section not found: " + section);
        }

        auto key_it = section_it->second.find(key);
        if (key_it == section_it->second.end()) {
            throw INIParserException("Key not found in section '" + section + "': " + key);
        }

        return convert_value<T>(key_it->second);
    }

    bool has_value(const std::string& path) const {
        try {
            size_t dot_pos = path.find('.');
            if (dot_pos == std::string::npos) return false;

            std::string section = path.substr(0, dot_pos);
            std::string key = path.substr(dot_pos + 1);

            auto section_it = data.find(section);
            if (section_it == data.end()) return false;

            auto key_it = section_it->second.find(key);
            return key_it != section_it->second.end();
        }
        catch (...) {
            return false;
        }
    }
};

int main() {
    try {
        std::ofstream test_file("test.ini");
        test_file << "[Section1]\n"
            << "; comment about section\n"
            << "var1=5.0 ; comment allowed for individual parameter\n"
            << "var2=some string\n"
            << "var3=42\n"
            << "  \n"
            << "[Section2]\n"
            << "var1=1\n"
            << "var2=value_2\n"
            << "var3=false\n"
            << "\n"
            << "; Sometimes there are no values, meaning Section3 has no variables\n"
            << "[Section3]\n"
            << "[Section4]\n"
            << "Mode=\n"
            << "Vid=\n"
            << "\n"
            << "; Sections can repeat\n"
            << "[Section1]\n"
            << "var3=value\n"
            << "var1=1.0 ; reassign value\n";
        test_file.close();

        ini_parser parser("test.ini");

        auto value1 = parser.get_value<double>("Section1.var1");
        std::cout << "Section1.var1 = " << value1 << std::endl;

        auto value2 = parser.get_value<std::string>("Section1.var2");
        std::cout << "Section1.var2 = " << value2 << std::endl;

        auto value3 = parser.get_value<int>("Section2.var1");
        std::cout << "Section2.var1 = " << value3 << std::endl;

        auto value4 = parser.get_value<std::string>("Section2.var2");
        std::cout << "Section2.var2 = " << value4 << std::endl;

        auto value5 = parser.get_value<std::string>("Section1.var3");
        std::cout << "Section1.var3 (last assignment) = " << value5 << std::endl;

        auto value6 = parser.get_value<std::string>("Section4.Mode");
        std::cout << "Section4.Mode = '" << value6 << "'" << std::endl;

        try {
            auto nonexistent = parser.get_value<int>("Section5.key");
        }
        catch (const INIParserException& e) {
            std::cout << "Expected error: " << e.what() << std::endl;
        }

        try {
            auto invalid_type = parser.get_value<int>("Section2.var2");
        }
        catch (const INIParserException& e) {
            std::cout << "Conversion error: " << e.what() << std::endl;
        }

    }
    catch (const INIParserException& e) {
        std::cerr << "Parser error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}