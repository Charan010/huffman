#include <bits/stdc++.h>


struct Node{
    char ch;
    int freq;
    Node* left;
    Node *right;

    Node(char c , int f){
        ch = c;
        freq = f;
        left = nullptr;
        right = nullptr;
    }

    Node(Node* l, Node *r){
        ch = '\0';
        freq = l -> freq + r -> freq;
        left = l;
        right = r; 
    }

};


struct Compare{

   bool operator()(Node* a, Node* b) {
        return a->freq > b->freq;
    }

};


/* using a priority queue to take minimum frequent character first and build tree. So, that characters
   with more frequency get shorter code instead of fixed code for all characters.

    For example:
        If my entire text/string uses only A,B,C,D,E
        then i need atleast 3 bits to encode characters.

        so total bits sent = 3 * length of the text/string.

    But according to information theory, I(x) is inversely proportional to P(x)
    So, more likely some event is to occur then less information we will gain.
   
    
*/

Node *build_huffman_tree(const std::unordered_map<char, int>&freq){

    std::priority_queue<Node*, std::vector<Node*>, Compare> pq;

    for(auto &[k, v] : freq)
        pq.push(new Node(k ,v));

    while(pq.size() > 1){
        Node *a = pq.top();
        pq.pop();

        Node *b = pq.top();
        pq.pop();

        pq.push(new Node(a, b));
    }

    // return root tree after building 
    return pq.top();
}


/* go recursively left and right and when hit the last character . add it to the char to bit string table. */
void build_codes(Node *root_node, std::string code, std::unordered_map<char, std::string>&table){
    if(!root_node)
        return;

    if(!root_node -> left && !root_node -> right){
        table[root_node -> ch] = code;
        return;
    }


    build_codes(root_node -> left, code + "0", table);
    build_codes(root_node -> right, code + "1", table);

}


/* using table as source of truth which contains mapping of each character to variable binary string */
std::string encode(const std::string &text, std::unordered_map<char, std::string> &table){

    std::string bits;
    for(char c : text){
        bits += table[c];
    }

    return bits;
}


std::string decode(const std::string &bits, Node *root){
    std::string out;
    Node *curr = root;


    for(char b : bits){
        curr = (b == '0') ? curr -> left : curr -> right;
        if(!curr -> left && !curr -> right){
            out += curr -> ch;
            curr = root;
        }
    }

    return out;

}

void benchmark(const std::string& text, const std::unordered_map<char, int>& freq, const std::unordered_map<char, std::string>& table,
    const std::string& encoded_bits){

    int naive_bits = 8 * text.size();

    int huffman_payload_bits = 0;
    for (auto& [ch, code] : table) {
        huffman_payload_bits += freq.at(ch) * code.size();
    }

    int n = freq.size();  
    
    /* header is to how we persist the table to disk so that it can decoded by other devices/person as well */
    int huffman_header_bits = (2 * n - 1)  + (8 * n);  

    int huffman_total_bits = huffman_payload_bits + huffman_header_bits;

    std::cout << "----------------------------------\n";
    std::cout << "Naive encoding:\n";
    std::cout << " Total bits = " << naive_bits << "\n\n";

    std::cout << "Huffman encoding:\n";
    std::cout << "  Payload bits = " << huffman_payload_bits << "\n";
    std::cout << "  Header bits  = " << huffman_header_bits << "\n";
    std::cout << "  Total bits   = " << huffman_total_bits << "\n\n";

    std::cout << "Compression ratio = " << (double)huffman_total_bits / naive_bits << "\n";
    std::cout << "----------------------------------\n";

}


int main(int argc, char* argv[]){

    if(argc < 2){   
        std::cerr << "Usage: " << "./huffman    " << " text" << "\n";
        return 1;
    }


    std::string text = argv[1];

    std::unordered_map<char,int> freq;
    for (char c : text)
        freq[c]++;

    std::vector<std::pair<char, int>> vv(freq.begin(), freq.end());

    std::sort(vv.begin(), vv.end(), [](const auto &a , const auto &b){
        return a.second > b.second;

    });

    Node* root = build_huffman_tree(freq);

    std::unordered_map<char, std::string> table;
    build_codes(root, "", table);

    std::string encoded = encode(text, table);
    std::string decoded = decode(encoded, root);


    int sz = 8 * freq.size();

    for(auto [k, v] : table)
        sz += (freq[k] * v.size());
    

    std::cout << "Encoded bits: " << encoded << "\n";

    benchmark(text, freq, table, encoded);

    return 0;

}










