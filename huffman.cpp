#include "huffman.h"
#include "bit_io.h"


Node::Node(char c, int f) {
    ch = c;
    freq = f;
    left = nullptr;
    right = nullptr;
}

Node::Node(Node* l, Node* r) {
    ch = '\0';
    freq = l->freq + r->freq;
    left = l;
    right = r;
}


bool Compare::operator()(Node* a, Node* b) {
    return a->freq > b->freq;
}


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
        table[root_node -> ch] = code.empty() ? "0" : code;
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

void write_tree(Node *node, BitWriter &bw){
    if(!node)
        return;

    if(!node -> left && !node -> right){
        bw.write_bit(1);
        bw.write_byte((uint8_t)node -> ch);
        return;
    }
    
    bw.write_bit(0);
    write_tree(node -> left, bw);
    write_tree(node -> right, bw);
}

Node* read_tree(BitReader &br){
    int bit = br.read_bit();

    if(bit == 1){
        char ch = (char)(br.read_byte());
        return new Node(ch, 0);
    }

    Node* left = read_tree(br);
    Node* right = read_tree(br);

    return new Node(left, right);
}


void free_tree(Node* node) {
    if (!node) return;
    free_tree(node->left);
    free_tree(node->right);
    delete node;
}

uint32_t read_uint32(BitReader& br) {
    uint32_t x = 0;
    for (int i = 0; i < 4; i++)
        x = (x << 8) | br.read_byte();
    return x;
}

std::string read_payload(BitReader& br, Node* root, uint32_t text_len) {
    std::string out;
    Node* curr = root;

    while (out.size() < text_len) {
        int bit = br.read_bit();
        curr = (bit == 0) ? curr->left : curr->right;

        if (!curr->left && !curr->right) {
            out.push_back(curr->ch);
            curr = root;
        }
    }

    return out;
}

std::string read_compressed_file(const std::string& file) {
    BitReader br(file);

    Node* root = read_tree(br);             
    uint32_t len = read_uint32(br);         
    return read_payload(br, root, len); 
}


void write_payload(const std::string& bits, BitWriter& bw) {
    for (char b : bits)
        bw.write_bit(b == '1');
}

void write_uint32(BitWriter& bw, uint32_t x) {
    bw.write_byte((x >> 24) & 0xFF);
    bw.write_byte((x >> 16) & 0xFF);
    bw.write_byte((x >> 8) & 0xFF);
    bw.write_byte(x & 0xFF);
}



void write_compressed_file(const std::string& file, Node* root, const std::string& encoded_bits, uint32_t original_len) {
    BitWriter bw(file);

    write_tree(root, bw);               
    write_uint32(bw, original_len);     
    write_payload(encoded_bits, bw);

    bw.flush();
}
