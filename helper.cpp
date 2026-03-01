#include "helper.h"


#include <string>
#include <fstream>      
#include <stdexcept>    
#include <filesystem>  
#include <iterator>  


std::string read_file(const std::string& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) throw std::runtime_error("Cannot open input file");

    return std::string(
        (std::istreambuf_iterator<char>(in)),
        std::istreambuf_iterator<char>()
    );
}


void write_file(const std::string& path, const std::string& data) {
    std::ofstream out(path, std::ios::binary);
    if (!out) throw std::runtime_error("Cannot open output file");

    out.write(data.data(), data.size());
}

std::string make_huf_name(const std::string& input) {
    namespace fs = std::filesystem;

    fs::path p(input);
    fs::path out = p.parent_path() /
                   (p.stem().string() + ".huf");

    return out.string();
}


