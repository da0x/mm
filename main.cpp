#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <filesystem>
#include <algorithm>
#include <sstream>
#include <optional>

namespace fs = std::filesystem;

using std::string;
using std::vector;
using std::map;
using std::ifstream;
using std::istringstream;
using std::runtime_error;
using std::ios;
using std::cout;
using std::endl;
using std::setw;
using std::optional;
using std::ofstream; // Add ofstream for file output

string compiler() {
    return "g++ -std=c++20";
}

string cmd(const string& command) {
    char buffer[128];
    string result;
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        throw runtime_error("popen() failed!");
    }
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }
    pclose(pipe);
    return result;
}

vector<string> split(const string& input) {
    vector<string> lines;
    istringstream stream(input);
    string line;
    while (std::getline(stream, line)) {
        lines.push_back(line);
    }
    return lines;
}

void find(const vector<fs::path>& files, vector<fs::path>& all, vector<fs::path>& cpp, vector<fs::path>& hpp) {
    for (const auto& file : files) {
        string ext = file.extension();
        if (ext == ".cpp") {
            cpp.push_back(file);
            all.push_back(file);
        } else if (ext == ".hpp") {
            hpp.push_back(file);
            all.push_back(file);
        }
    }
}

string binary() {
    fs::path path = fs::current_path();
    return path.stem();
}

string path(const string& name) {
    return ".obj/" + fs::path(name).stem().string() + ".o";
}

string tab(int count) {
    string tabs;
    for (int i = 0; i < count; i++) {
        tabs += "\t";
    }
    return tabs;
}

int main(int argc, char* argv[]) {
    optional<string> output_file_name;
    if (argc == 2) {
        output_file_name = string(argv[1]);
    }
    ofstream outputFile;
    if (output_file_name) {
        outputFile.open(*output_file_name);
    }

    std::ostream& output = output_file_name ? outputFile : cout;

    map<string, vector<string>> dependencies;
    vector<fs::path> files;

    for (const auto& entry : fs::directory_iterator(".")) {
        files.push_back(entry.path());
    }

    vector<fs::path> all, cpp, hpp;
    find(files, all, cpp, hpp);

    for (const auto& file : all) {
        ifstream input(file);
        vector<string> lines;
        for (string line; std::getline(input, line);) {
            lines.push_back(line);
        }

        vector<string> inc;
        for (const auto& other : all) {
            if (file != other) {
                for (const string& line : lines) {
                    if (line.find(other.string()) != string::npos) {
                        inc.push_back(other.string());
                        break;
                    }
                }
            }
        }

        dependencies[file.string()] = inc;
    }

    output << "# Generated with github.com/da0x/mmpp" << endl;
    output << "# Binary:" << endl;
    output << binary() << ": .obj";
    for (const fs::path& f : cpp) {
        output << " " << path(f.string());
    }
    output << endl;
    output << tab(1) << compiler();
    for (const fs::path& f : cpp) {
        output << " " << path(f.string());
    }
    output << " -o " << binary() << endl;
    output << endl;

    output << ".obj:" << endl;
    output << tab(1) << "mkdir .obj" << endl;
    output << endl;
    for (const fs::path& c : cpp) {
        output << path(c.string()) << ": " << c.string();
        for (const string& d : dependencies[c.string()]) {
            output << " " << d;
        }
        output << endl;
        output << tab(1) << compiler() << " -o " << path(c.string()) << " -c " << c.string() << endl;
        output << endl;
    }

    output << "run: " << binary() << endl;
    output << tab(1) << "./" << binary() << endl;
    output << endl;
    output << "clean:" << endl;
    output << tab(1) << "rm -rfv .obj" << endl;
    output << endl;
    output << "install:" << endl;
    output << tab(1) << "sudo cp -v " << binary() << " /usr/local/bin/" << endl;
    output << endl;
    output << "uninstall:" << endl;
    output << tab(1) << "sudo rm -v /usr/local/bin/" << binary() << endl;

    return 0;
}

