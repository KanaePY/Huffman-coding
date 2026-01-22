# Huffman Coding - Text Compression Tool

A C++ implementation of the Huffman Coding algorithm used for lossless data compression.

## 📌 Project Overview
This tool compresses text files by generating variable-length binary codes for characters based on their frequencies. The more frequent a character, the shorter its code, resulting in a reduced overall file size.

### Features
* **Frequency Analysis:** Scans input text to determine character frequency.
* **Min-Priority Queue:** Uses a heap-based priority queue to build the Huffman Tree.
* **Encoding:** Generates a prefix-free binary mapping for each character.
* **Compression:** Outputs a compressed version of the input file.
* **Decompression:** Reconstructs the original text from the compressed data.

## 🛠️ Data Structures Used
* **Huffman Tree (Binary Tree):** For storing character codes.
* **Priority Queue (Min-Heap):** To efficiently build the tree from the bottom up.
* **Hash Map / Frequency Table:** To store character occurrences.

## 🚀 Getting Started

### Prerequisites
* A C++ compiler (GCC/G++, MinGW, or Clang).
* Git (to clone the repo).

## Execution
```bash
./HuffmanCoding -c input.txt output.huf
./HuffmanCoding -d output.huf REinput.txt
```
