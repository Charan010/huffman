A C++ implementation of the Huffman encoding algorithm that takes text as input and encodes
data using shorter codes to reduce required number of bits.

## How It Works

According to information theory, the more uncertain an event is to occur, the more information we will gain. So I(x) ∝ 1/P(x).

So, using this encoding, more frequent characters get shorter binary code so, total number of bits used will reduce drastically for huge files.

## Usage

```bash
mkdir build
cd huffman/build
cmake ..
cmake --build .
./huffman "text to be passed here"
```

### TO-DO

- Accept a file path as input instead of a raw string
- Persist the Huffman code table / tree to disk so the encoded file can be decoded later


## Requirements

- C++17 or later
- CMake 3.10+