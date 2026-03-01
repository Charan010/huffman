A C++ implementation of the Huffman encoding algorithm that takes text as input and encodes
data using shorter codes to reduce required number of bits.

## How It Works

According to information theory, the more uncertain an event is to occur, the more information we will gain. So I(x) ∝ 1/P(x).

So, using this encoding, more frequent characters get shorter binary code so, total number of bits used will reduce drastically for huge files.

## How to install:

```bash
cmake -S . -B build
cmake --build build

cmake --install build

```
## Usage:

```bash
huffman -c <absolute-path-to-the-file> (to encode)
huffman -d <file-to-be-decoded> <file-to-be-dumped> 

```


## Requirements

- C++17 or later
- CMake 3.10+