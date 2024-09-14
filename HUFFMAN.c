#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TREE_HT 100
#define BUFFER_SIZE 1024

// Huffman Tree Node
struct MinHeapNode {
    char data;
    unsigned freq;
    struct MinHeapNode *left, *right;
};

// Min Heap
struct MinHeap {
    unsigned size;
    unsigned capacity;
    struct MinHeapNode** array;
};

// Functions from the previous implementation...

struct MinHeapNode* newNode(char data, unsigned freq) {
    struct MinHeapNode* temp = (struct MinHeapNode*)malloc(sizeof(struct MinHeapNode));
    temp->left = temp->right = NULL;
    temp->data = data;
    temp->freq = freq;
    return temp;
}

struct MinHeap* createMinHeap(unsigned capacity) {
    struct MinHeap* minHeap = (struct MinHeap*)malloc(sizeof(struct MinHeap));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (struct MinHeapNode**)malloc(minHeap->capacity * sizeof(struct MinHeapNode*));
    return minHeap;
}

void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b) {
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

void minHeapify(struct MinHeap* minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq)
        smallest = left;

    if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq)
        smallest = right;

    if (smallest != idx) {
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

int isSizeOne(struct MinHeap* minHeap) {
    return (minHeap->size == 1);
}

struct MinHeapNode* extractMin(struct MinHeap* minHeap) {
    struct MinHeapNode* temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    --minHeap->size;
    minHeapify(minHeap, 0);
    return temp;
}

void insertMinHeap(struct MinHeap* minHeap, struct MinHeapNode* minHeapNode) {
    ++minHeap->size;
    int i = minHeap->size - 1;
    while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->array[i] = minHeapNode;
}

void buildMinHeap(struct MinHeap* minHeap) {
    int n = minHeap->size - 1;
    for (int i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}

int isLeaf(struct MinHeapNode* root) {
    return !(root->left) && !(root->right);
}

struct MinHeap* createAndBuildMinHeap(char data[], int freq[], int size) {
    struct MinHeap* minHeap = createMinHeap(size);
    for (int i = 0; i < size; ++i)
        minHeap->array[i] = newNode(data[i], freq[i]);
    minHeap->size = size;
    buildMinHeap(minHeap);
    return minHeap;
}

struct MinHeapNode* buildHuffmanTree(char data[], int freq[], int size) {
    struct MinHeapNode *left, *right, *top;
    struct MinHeap* minHeap = createAndBuildMinHeap(data, freq, size);
    while (!isSizeOne(minHeap)) {
        left = extractMin(minHeap);
        right = extractMin(minHeap);
        top = newNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;
        insertMinHeap(minHeap, top);
    }
    return extractMin(minHeap);
}

void getCodes(struct MinHeapNode* root, int arr[], int top, char codes[256][MAX_TREE_HT]) {
    if (root->left) {
        arr[top] = 0;
        getCodes(root->left, arr, top + 1, codes);
    }
    if (root->right) {
        arr[top] = 1;
        getCodes(root->right, arr, top + 1, codes);
    }
    if (isLeaf(root)) {
        for (int i = 0; i < top; i++) {
            codes[(int)root->data][i] = arr[i] + '0';
        }
        codes[(int)root->data][top] = '\0';
    }
}

// Compress the file
void compressFile(const char* inputFilename, const char* outputFilename) {
    FILE* inputFile = fopen(inputFilename, "rb");
    if (!inputFile) {
        printf("Error opening input file.\n");
        return;
    }

    // Frequency table
    int freq[256] = {0};
    char ch;

    // Calculate frequencies
    while ((ch = fgetc(inputFile)) != EOF) {
        freq[(unsigned char)ch]++;
    }
    rewind(inputFile);

    // Build Huffman Tree
    char data[256];
    int dataFreq[256];
    int size = 0;
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            data[size] = (char)i;
            dataFreq[size] = freq[i];
            size++;
        }
    }
    struct MinHeapNode* root = buildHuffmanTree(data, dataFreq, size);

    // Generate codes
    char codes[256][MAX_TREE_HT] = {{0}};
    int arr[MAX_TREE_HT], top = 0;
    getCodes(root, arr, top, codes);

    // Write the Huffman tree and data to the output file
    FILE* outputFile = fopen(outputFilename, "wb");
    if (!outputFile) {
        printf("Error opening output file.\n");
        return;
    }

    // Write frequency table to the output file for decompression
    fwrite(freq, sizeof(int), 256, outputFile);

    // Compress and write the data to the file
    char buffer[BUFFER_SIZE] = {0};
    int bufferIndex = 0;
    while ((ch = fgetc(inputFile)) != EOF) {
        char* code = codes[(unsigned char)ch];
        for (int i = 0; code[i] != '\0'; i++) {
            buffer[bufferIndex++] = code[i];
            if (bufferIndex == 8) {
                char byte = 0;
                for (int j = 0; j < 8; j++) {
                    if (buffer[j] == '1') {
                        byte |= (1 << (7 - j));
                    }
                }
                fputc(byte, outputFile);
                bufferIndex = 0;
            }
        }
    }

    // Flush remaining bits
    if (bufferIndex > 0) {
        char byte = 0;
        for (int i = 0; i < bufferIndex; i++) {
            if (buffer[i] == '1') {
                byte |= (1 << (7 - i));
            }
        }
        fputc(byte, outputFile);
    }

    fclose(inputFile);
    fclose(outputFile);
}

void decompressFile(const char* inputFilename, const char* outputFilename) {
    FILE* inputFile = fopen(inputFilename, "rb");
    if (!inputFile) {
        printf("Error opening input file.\n");
        return;
    }

    // Read the frequency table from the file
    int freq[256] = {0};
    fread(freq, sizeof(int), 256, inputFile);

    // Rebuild the Huffman Tree from the frequency table
    char data[256];
    int dataFreq[256];
    int size = 0;
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            data[size] = (char)i;
            dataFreq[size] = freq[i];
            size++;
        }
    }
    struct MinHeapNode* root = buildHuffmanTree(data, dataFreq, size);

    // Open the output file to write the decompressed data
    FILE* outputFile = fopen(outputFilename, "wb");
    if (!outputFile) {
        printf("Error opening output file.\n");
        fclose(inputFile);
        return;
    }

    // Traverse the Huffman Tree and decode the file
    struct MinHeapNode* current = root;
    char byte;
    while (fread(&byte, sizeof(char), 1, inputFile) == 1) {
        // Process each bit of the byte
        for (int i = 7; i >= 0; i--) {
            // Check the current bit
            if (byte & (1 << i)) {
                current = current->right; // Bit is 1, move to the right
            } else {
                current = current->left;  // Bit is 0, move to the left
            }

            // If we reach a leaf node, write the character to the output file
            if (isLeaf(current)) {
                fputc(current->data, outputFile);
                current = root; // Go back to the root to decode the next character
            }
        }
    }

    // Close the input and output files
    fclose(inputFile);
    fclose(outputFile);
}

int main(int argc, char *argv[]) {
    // Check if the number of arguments is correct
    if (argc != 4) {
        printf("Usage: %s <option> <input_file> <output_file>\n", argv[0]);
        printf("Options:\n");
        printf("  -c    Compress the input file\n");
        printf("  -d    Decompress the input file\n");
        return 1;
    }

    // Determine whether to compress or decompress based on the option
    if (strcmp(argv[1], "-c") == 0) {
        // Compress the file
        printf("Compressing file: %s\n", argv[2]);
        compressFile(argv[2], argv[3]);
        printf("File compressed and saved as: %s\n", argv[3]);
    } else if (strcmp(argv[1], "-d") == 0) {
        // Decompress the file
        printf("Decompressing file: %s\n", argv[2]);
        decompressFile(argv[2], argv[3]);
        printf("File decompressed and saved as: %s\n", argv[3]);
    } else {
        // Invalid option
        printf("Invalid option: %s\n", argv[1]);
        printf("Usage: %s <option> <input_file> <output_file>\n", argv[0]);
        printf("Options:\n");
        printf("  -c    Compress the input file\n");
        printf("  -d    Decompress the input file\n");
        return 1;
    }

    return 0;
}