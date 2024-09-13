#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <filesystem>
#include <set>
#include <regex>
#include <optional>

using namespace std;
namespace fs = filesystem;

string compiler() {
    return "g++ -std=c++20";
}

void find_files(const fs::path& directory, vector<fs::path>& all, vector<fs::path>& cpp_files, vector<fs::path>& hpp_files) {
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (entry.is_directory()) {
            find_files(entry.path(), all, cpp_files, hpp_files);
        } else {
            fs::path file = entry.path();
            string ext = file.extension();
            if (ext == ".cpp") {
                cpp_files.push_back(file);
            } else if (ext == ".hpp" || ext == ".h") {
                hpp_files.push_back(file);
            }
            all.push_back(file);
        }
    }
}

string binary_name() {
    fs::path path = fs::current_path();
    return path.stem();
}

string obj_path(const string& name) {
    return ".obj/" + fs::path(name).filename().string() + ".o";
}

string generate_tab(int count) {
    string tabs;
    for (int i = 0; i < count; i++) {
        tabs += "\t";
    }
    return tabs;
}

int main(int argc, char* argv[]) {
    std::optional<string> output_file_name;
    if (argc == 2) {
        output_file_name = string(argv[1]);
    }
    ofstream output_file;
    if (output_file_name) {
        output_file.open(*output_file_name);
    }
    fs::path base_path = fs::current_path();
    ostream& output = output_file_name ? output_file : cout;
    map<string, vector<string>> dependencies;
    vector<fs::path> files, cpp_files, hpp_files;

    find_files(fs::current_path(), files, cpp_files, hpp_files);

    set<string> header_file_paths;
    for (const auto& hpp : hpp_files) {
        header_file_paths.insert(hpp.string());
    }

    output << "# Generated with github.com/da0x/mmpp" << endl;
    output << "# Binary:" << endl;
    output << binary_name() << ": .obj";
    for (const fs::path& f : cpp_files) {
        output << " " << obj_path(f.string());
    }
    output << endl;
    output << generate_tab(1) << compiler();
    for (const fs::path& f : cpp_files) {
        output << " " << obj_path(f.string());
    }
    output << " -o " << binary_name() << endl;
    output << endl;

    output << ".obj:" << endl;
    output << generate_tab(1) << "mkdir .obj" << endl;
    output << endl;

    for (const fs::path& c : cpp_files) {
        output << obj_path(c.filename().string()) << ": " << c.filename().string();
        for (const string& d : dependencies[c.filename().string()]) {
            output << " " << d;
        }
        output << endl;
        output << generate_tab(1) << compiler() << " -o " << obj_path(c.filename().string()) << " -c " << c.filename().string() << endl;
        output << endl;
    }

    output << "run: " << binary_name() << endl;
    output << generate_tab(1) << "./" << binary_name() << endl;
    output << endl;

    output << "clean:" << endl;
    output << generate_tab(1) << "rm -rfv .obj" << endl;
    output << endl;

    output << "install: " << binary_name() << endl;
    output << generate_tab(1) << "sudo cp -v " << binary_name() << " /usr/local/bin/" << endl;
    output << endl;

    output << "uninstall:" << endl;
    output << generate_tab(1) << "sudo rm -v /usr/local/bin/" << binary_name() << endl;

    return 0;
}

