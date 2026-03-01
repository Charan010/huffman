#include "bit_io.h"

BitWriter::BitWriter(const std::string& file)
    : out(file, std::ios::binary) {}

void BitWriter::write_bit(int b){
    buffer = (buffer << 1) | b;
    bits++;

    if(bits == 8){
        out.put(buffer);
        buffer = 0;
        bits = 0;
    }
}

void BitWriter::write_byte(uint8_t b){
    if(bits == 0)
        out.put(b);

    else
        for(int i = 7; i >= 0 ; i--)
            write_bit((b >> i) & 1);
}


void BitWriter::flush(){
    if(bits > 0){
        buffer <<= (8 - bits);
        out.put(buffer);

        buffer = 0 ;
        bits = 0;
    }
}

BitReader::BitReader(const std::string& file)
    : in(file, std::ios::binary) {}



int BitReader::read_bit(){
    if(bits == 0){
        buffer = in.get();
        bits = 8;
    }

    int bit = (buffer >> 7) & 1;
    buffer <<= 1;

    bits--;
    return bit;
}

uint8_t BitReader::read_byte(){
    uint8_t b = 0;
    for(int i = 0 ; i < 8 ; ++i)
        b = (b << 1) | read_bit();

    return b;
}





