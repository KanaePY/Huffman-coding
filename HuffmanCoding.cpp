#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

struct Node {
    char ch;
    int freq;
    Node *left, *right;
    
    Node(char c, int f) : ch(c), freq(f), left(NULL), right(NULL) {}
};

class HashMap {
private:
    static const int SIZE = 256;
    struct Entry {
        char key;
        int value;
        bool used;
        Entry() : key(0), value(0), used(false) {}
    };
    Entry table[SIZE];
    
public:
    void put(char key, int value) {
        int index = (unsigned char)key;
        table[index].key = key;
        table[index].value = value;
        table[index].used = true;
    }
    
    int get(char key) {
        int index = (unsigned char)key;
        return table[index].used ? table[index].value : 0;
    }
    
    bool contains(char key) {
        int index = (unsigned char)key;
        return table[index].used;
    }
    
    void clear() {
        for (int i = 0; i < SIZE; i++) {
            table[i].used = false;
        }
    }
    
    char* getKeys(int& count) {
        count = 0;
        for (int i = 0; i < SIZE; i++) {
            if (table[i].used) count++;
        }
        
        char* keys = new char[count];
        int index = 0;
        for (int i = 0; i < SIZE; i++) {
            if (table[i].used) {
                keys[index++] = table[i].key;
            }
        }
        return keys; 
    }
};

class MinHeap {
private:
    Node** arr;
    int capacity;
    int size;
    
    void heapifyUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (arr[index]->freq < arr[parent]->freq) {
                Node* temp = arr[index];
                arr[index] = arr[parent];
                arr[parent] = temp;
                index = parent;
            } else {
                break;
            }
        }
    }
    
    void heapifyDown(int index) {
        while (index < size) {
            int smallest = index;
            int left = 2 * index + 1;
            int right = 2 * index + 2;
            
            if (left < size && arr[left]->freq < arr[smallest]->freq) {
                smallest = left;
            }
            if (right < size && arr[right]->freq < arr[smallest]->freq) {
                smallest = right;
            }
            
            if (smallest != index) {
                Node* temp = arr[index];
                arr[index] = arr[smallest];
                arr[smallest] = temp;
                index = smallest;
            } else {
                break;
            }
        }
    }
    
public:
    MinHeap(int cap) : capacity(cap), size(0) {
        arr = new Node*[capacity];
    }
    
    void push(Node* node) {
        if (size >= capacity) return;
        arr[size] = node;
        heapifyUp(size);
        size++;
    }
    
    Node* top() {
        return size > 0 ? arr[0] : NULL;
    }
    
    void pop() {
        if (size == 0) return;
        arr[0] = arr[size - 1];
        size--;
        heapifyDown(0);
    }
    
    int getSize() { return size; }
    
    ~MinHeap() { delete[] arr; }
};

class CodeMap {
private:
    static const int SIZE = 256;
    char* codes[SIZE];
    
public:
    CodeMap() {
        for (int i = 0; i < SIZE; i++) {
            codes[i] = NULL;
        }
    }
    
    void put(char key, const char* code) {
        int index = (unsigned char)key;
        if (codes[index]) delete[] codes[index];
        
        codes[index] = new char[strlen(code) + 1];
        strcpy(codes[index], code);
    }
    
    const char* get(char key) {
        int index = (unsigned char)key;
        return codes[index];
    }
    
    ~CodeMap() {
        for (int i = 0; i < SIZE; i++) {
            if (codes[i]) delete[] codes[i];
        }
    }
};

class HuffmanCoding {
private:
    CodeMap codes;
    Node* root;
    
    Node* buildTree(const char* text, int length) {
        HashMap freq;
        for (int i = 0; i < length; i++) {
            char c = text[i];
            freq.put(c, freq.get(c) + 1);
        }
        
        int uniqueCount;
        char* uniqueChars = freq.getKeys(uniqueCount);
        
        MinHeap pq(uniqueCount);
        for (int i = 0; i < uniqueCount; i++) {
            pq.push(new Node(uniqueChars[i], freq.get(uniqueChars[i])));
        }
        delete[] uniqueChars;
        
        while (pq.getSize() > 1) {
            Node* left = pq.top(); pq.pop();
            Node* right = pq.top(); pq.pop();
            
            Node* parent = new Node('\0', left->freq + right->freq);
            parent->left = left;
            parent->right = right;
            pq.push(parent);
        }
        
        return pq.top();
    }
    
    void generateCodes(Node* node, char* code, int depth) {
        if (!node) return;
        
        if (!node->left && !node->right) {
            code[depth] = '\0';
            codes.put(node->ch, depth == 0 ? "0" : code);
            return;
        }
        
        code[depth] = '0';
        generateCodes(node->left, code, depth + 1);
        
        code[depth] = '1';
        generateCodes(node->right, code, depth + 1);
    }
    
    void writeTree(Node* node, ofstream& out) {
        if (!node) return;
        
        if (!node->left && !node->right) {
            out.put('1');
            out.put(node->ch);
        } else {
            out.put('0');
            writeTree(node->left, out);
            writeTree(node->right, out);
        }
    }
    
    Node* readTree(ifstream& in) {
        char marker;
        in.get(marker);
        
        if (marker == '1') {
            char ch;
            in.get(ch);
            return new Node(ch, 0);
        }
        
        Node* node = new Node('\0', 0);
        node->left = readTree(in);
        node->right = readTree(in);
        return node;
    }
    
public:
    HuffmanCoding() : root(NULL) {}
    
    bool compress(const char* inputFile, const char* outputFile) {
        ifstream in(inputFile, ios::binary);
        if (!in) {
            cout << "Error: Cannot open " << inputFile << endl;
            return false;
        }
        
        in.seekg(0, ios::end);
        int length = in.tellg();
        in.seekg(0, ios::beg);
        
        char* text = new char[length + 1];
        in.read(text, length);
        text[length] = '\0';
        in.close();
        
        if (length == 0) {
            cout << "Error: File is empty" << endl;
            delete[] text;
            return false;
        }
        
        root = buildTree(text, length);
        char code[256];
        generateCodes(root, code, 0);
        
        char* encoded = new char[length * 8 + 1];
        encoded[0] = '\0';
        
        for (int i = 0; i < length; i++) {
            strcat(encoded, codes.get(text[i]));
        }
        delete[] text;
        
        int encodedLen = strlen(encoded);
        
        ofstream out(outputFile, ios::binary);
        writeTree(root, out);
        
        int padding = (8 - encodedLen % 8) % 8;
        out.put(padding);

        for (int i = 0; i < padding; i++) {
            strcat(encoded, "0");
        }
        
        for (int i = 0; i < encodedLen + padding; i += 8) {
            unsigned char byte = 0;
            for (int j = 0; j < 8; j++) {
                byte = (byte << 1) | (encoded[i + j] - '0');
            }
            out.put(byte);
        }
        
        delete[] encoded;
        out.close();

        ifstream origFile(inputFile, ios::binary | ios::ate);
        ifstream compFile(outputFile, ios::binary | ios::ate);
        
        long originalSize = origFile.tellg();
        long compressedSize = compFile.tellg();
        double ratio = (1.0 - (double)compressedSize / originalSize) * 100;
        
        origFile.close();
        compFile.close();
        
        cout << "Compression successful!" << endl;
        cout << "Original size: " << originalSize << " bytes" << endl;
        cout << "Compressed size: " << compressedSize << " bytes" << endl;
        cout << "Compression ratio: " << ratio << "%" << endl;
        
        cout << "Compressed to " << outputFile << endl;
        return true;
    }
    
    bool decompress(const char* inputFile, const char* outputFile) {
        ifstream in(inputFile, ios::binary);
        if (!in) {
            cout << "Error: Cannot open " << inputFile << endl;
            return false;
        }
        
        root = readTree(in);
        
        char paddingChar;
        in.get(paddingChar);
        int padding = (unsigned char)paddingChar;
        
        char* encoded = new char[1000000];
        encoded[0] = '\0';
        
        char byte;
        while (in.get(byte)) {
            for (int i = 7; i >= 0; i--) {
                strcat(encoded, ((byte >> i) & 1) ? "1" : "0");
            }
        }
        in.close();
        
        int encodedLen = strlen(encoded);
        
        if (padding > 0) {
            encoded[encodedLen - padding] = '\0';
            encodedLen -= padding;
        }
        
        char* decoded = new char[encodedLen + 1];
        int decodedIndex = 0;
        Node* curr = root;
        
        for (int i = 0; i < encodedLen; i++) {
            curr = (encoded[i] == '0') ? curr->left : curr->right;
            
            if (!curr->left && !curr->right) {
                decoded[decodedIndex++] = curr->ch;
                curr = root;
            }
        }
        decoded[decodedIndex] = '\0';
        
        delete[] encoded;
        
        ofstream out(outputFile, ios::binary);
        out.write(decoded, decodedIndex);
        out.close();
        
        delete[] decoded;
        
        cout << "Decompressed to " << outputFile << endl;
        return true;
    }
};

int main(int argc, char* argv[]) {
    if (argc < 4) {
        cout << "Usage:\n";
        cout << "  Compress:   " << argv[0] << " -c input.txt output.huf\n";
        cout << "  Decompress: " << argv[0] << " -d input.huf output.txt\n";
        return 1;
    }
    
    HuffmanCoding hc;
    
    if (strcmp(argv[1], "-c") == 0) {
        hc.compress(argv[2], argv[3]);
    } else if (strcmp(argv[1], "-d") == 0) {
        hc.decompress(argv[2], argv[3]);
    } else {
        cout << "Invalid mode. Use -c or -d\n";
        return 1;
    }
    
    return 0;
}