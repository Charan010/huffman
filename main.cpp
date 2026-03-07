#include <iostream>
#include <unordered_map>
#include "huffman.h"
#include "bit_io.h"
#include <array>
#include "helper.h"
#include <filesystem>

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {


    Node *root;

    if (argc < 3) {
        std::cerr << "Usage:\n" " Encode: ./huffman -c <input_file>\n"
            "  Decode: ./huffman -d <input_file.huf> <output_file>\n";
        return 1;
    }

    std::string mode = argv[1];

    try {
        if (mode == "-c") {

            fs::path input_path  = fs::absolute(argv[2]);
            fs::path output_path = input_path.parent_path() / (input_path.stem().string() + ".huf");

            std::string text = read_file(input_path.string());

            std::array<uint32_t, 256> freq = {0};

            for(unsigned char c : text)
                freq[c]++;

            root = build_huffman_tree(freq);

            std::unordered_map<char, std::string> table;
            build_codes(root, "", table);

            std::string encoded_bits = encode(text, table);

            write_compressed_file(output_path.string(),root,encoded_bits,text.size());
            std::cout << "Compressed -> " << output_path.string() << "\n";

        }

        else if (mode == "-d"){

            if (argc < 4) {
                std::cerr << "Decode requires output file path\n";
                return 1;
            }

            fs::path input_path  = fs::absolute(argv[2]);
            fs::path output_path = fs::absolute(argv[3]);

            std::string decoded = read_compressed_file(input_path.string());
            write_file(output_path.string(), decoded);

            std::cout << "Decompressed -> " << output_path.string() << "\n";
        }

        else {
            std::cerr << "Unknown mode: " << mode << "\n";
            return 1;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
  
    return 0;
}