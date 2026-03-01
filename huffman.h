#pragma once

#include <unordered_map>
#include <string>
#include <queue>
#include <vector>
#include <iostream>
#include <cstdint>


struct Node {
    char ch;
    int freq;
    Node* left;
    Node* right;

     Node(char c, int f);
     Node(Node* l, Node* r);
};

struct Compare {
    bool operator()(Node* a, Node* b);
};


Node* build_huffman_tree(const std::unordered_map<char, int>& freq);

void free_tree(Node *root);

void build_codes(Node* root_node, std::string code, std::unordered_map<char, std::string>& table);

std::string encode(const std::string& text, std::unordered_map<char, std::string>& table);


std::string decode(const std::string& bits,Node* root);

void benchmark(const std::string& text, const std::unordered_map<char, int>& freq, const std::unordered_map<char, std::string>& table, 
    const std::string& encoded_bits);

void write_compressed_file(const std::string& file, Node* root, const std::string& encoded_bits, 
    uint32_t original_len);

std::string read_compressed_file(const std::string& file);
