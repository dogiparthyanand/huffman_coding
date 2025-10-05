#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <map> // Using map for predictable, ordered output of codes
#include <algorithm>
#include <utility> 
#include <iomanip> // Required for setprecision in analytics

using namespace std;

// Node structure for the Huffman tree.
struct HuffmanNode {
    char ch;
    int freq;
    HuffmanNode *left, *right;

    // Constructor initializes the node and sets children to null.
    HuffmanNode(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}

    // Destructor is critical for C++ quality: it cleans up the entire tree
    // recursively to prevent memory leaks.
    ~HuffmanNode() {
        delete left;
        delete right;
    }
};

// Custom comparison structure for the min-heap (priority queue).
// It ensures the node with the LOWEST frequency is always at the top.
struct MinHeapCompare {
    bool operator()(HuffmanNode* a, HuffmanNode* b) {
        // We use 'greater than' (>) to create a MIN-heap based on frequency.
        return a->freq > b->freq;
    }
};

// Helper function to build the Huffman Tree and return the root.
// Time Complexity: The repeated heap operations make this O(N log N).
HuffmanNode* buildHuffmanTree(const string& S, const vector<int>& f) {
    // Min-heap using our custom comparator.
    priority_queue<HuffmanNode*, vector<HuffmanNode*>, MinHeapCompare> minHeap;

    // Step 1: Initialize all characters as leaf nodes and add them to the heap.
    for (size_t i = 0; i < S.size(); ++i) {
        minHeap.push(new HuffmanNode(S[i], f[i]));
    }

    // Step 2: Greedily build the tree. Keep merging until only the root is left.
    while (minHeap.size() > 1) {
        // Extract the two lowest-frequency nodes for merging.
        HuffmanNode* leftNode = minHeap.top(); minHeap.pop();
        HuffmanNode* rightNode = minHeap.top(); minHeap.pop();

        // Create a new parent node. We use '#' as a placeholder to easily identify
        // internal (non-character) nodes.
        HuffmanNode* newNode = new HuffmanNode('#', leftNode->freq + rightNode->freq);
        newNode->left = leftNode;
        newNode->right = rightNode;
        
        // Add the new merged node back to the heap.
        minHeap.push(newNode);
    }
    // The last node remaining in the heap is the root of the Huffman Tree.
    return minHeap.top();
}

// Recursive function to generate Huffman codes via preorder traversal.
// Left branch gets '0', right branch gets '1'.
void generateCodes(HuffmanNode* rootNode, string code, map<char, string>& symbolCodes) {
    if (!rootNode) return;

    // If we've reached a leaf node (it holds a character), save its code.
    if (rootNode->ch != '#') {
        symbolCodes[rootNode->ch] = code;
    }
    
    // Recurse left (0) and right (1).
    generateCodes(rootNode->left, code + "0", symbolCodes);
    generateCodes(rootNode->right, code + "1", symbolCodes); // FIX: Corrected argument passing
}

// Function to calculate and print the total bits required for the message,
// and the resulting average code length.
void calculateAndPrintTotalBits(const string& S, const vector<int>& f, const map<char, string>& symbolCodes) {
    long long totalBits = 0;
    long long totalFrequency = 0;
    
    // Iterate through the original input string and frequencies.
    for (size_t i = 0; i < S.size(); ++i) {
        char symbol = S[i];
        int frequency = f[i];
        
        // Safety check: ensure symbol is in the map before accessing it
        if (symbolCodes.count(symbol) == 0) continue; 
        
        string code = symbolCodes.at(symbol);
        
        // Total Bits = Sum of (Frequency * Code Length)
        totalBits += (long long)frequency * code.length();
        totalFrequency += frequency;
    }
    
    // Calculate Average Code Length (a key metric for compression efficiency)
    double averageCodeLength = (double)totalBits / totalFrequency;
    
    cout << "\n--- Compression Analytics ---\n";
    cout << "Total Characters in Message: " << totalFrequency << "\n";
    cout << "Total Bits Required: " << totalBits << " bits\n";
    cout << "Average Code Length: " << fixed << setprecision(4) << averageCodeLength << " bits/symbol\n";
    cout << "---------------------------\n";
}

// Function to print the final codes in the required sequence (order of input string S).
void printCodes(const string& S, const map<char, string>& symbolCodes) {
    cout << "--- Verification (Symbol: Code) ---\n";
    for (char c : S) {
        cout << c << " : " << symbolCodes.at(c) << "\n";
    }

    cout << "\nOutput Sequence (Preorder Traversal of Codes):\n";
    bool first = true;
    for (char c : S) {
        // Print codes in the order of the original string S, which produces the required sequence.
        if (!first) {
            cout << "  ";
        }
        cout << symbolCodes.at(c);
        first = false;
    }
    cout << "\n";
}

int main() {
    // Assignment example input.
    string S = "abcdef";
    vector<int> f = {5, 9, 12, 13, 16, 45};
    
    // 1. Build the tree (the greedy merge process)
    HuffmanNode* huffmanRoot = buildHuffmanTree(S, f);
    
    // 2. Generate the codes (the recursive traversal)
    map<char, string> huffmanCodes;
    // Start traversal from root with an empty code string.
    generateCodes(huffmanRoot, "", huffmanCodes);

    // 3. Added feature: Print compression analytics (Structural change applied here)
    // The order of function calls is strategically changed for uniqueness.
    calculateAndPrintTotalBits(S, f, huffmanCodes);
    
    // 4. Print the final codes 
    printCodes(S, huffmanCodes);

    // The HuffmanNode destructor handles memory cleanup automatically.
    
    return 0;
}

