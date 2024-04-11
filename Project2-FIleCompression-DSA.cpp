/*
JHONDEL MICO N. ABAS
CHRISTIAN DAYNE R. MENDERICO
BSIT-2A
FILE COMPRESSION
*/
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
const int NUM_CHARACTERS = 256;
// Define a node structure for characters and their frequencies
struct NodeData{
    char data;                  // stores the character from the text file
    int frequency;              // stores the frequency of each character
    NodeData* left;             // pointer to the left of the node
    NodeData* right;            // pointer to the right of the node
    NodeData(char c, int freq) : data(c), frequency(freq), left(NULL),right(NULL){} // constructor
};
struct Node {                   // node structure to store the nodeData
    NodeData *node;//binary tree pointer
    Node* next;
    Node(NodeData* bn): node(bn), next(NULL){}  // constructor
};

class PriorityQueue {
private:
    Node* head;
    size_t originalLength; // stores the original length of the binary string
public:
    PriorityQueue() : head(NULL) {} // initialize head to NULL
    // Function to insert a character with its frequency into the priority queue
    void enqueue(char data, int frequency) {
        NodeData* node = new NodeData(data, frequency); //binary tree node
        Node* newNode = new Node(node); // stores the node pointer to Node structure

        // If the list is empty or the new node has lower frequency than the head, insert at the beginning
        if (!head || newNode->node->frequency < head->node->frequency) {
            newNode->next = head; // moves the current head as the 2nd on the list
            head = newNode; // sets the newNode as the head of the list 
        } else {
            // Find the correct position to insert the new node
            Node* current = head;
            while (current->next && newNode->node->frequency >= current->next->node->frequency) {
                current = current->next; // traverse the list until the frequency of the newNode is greater than or equal to the current node in the traversal
            }
            newNode->next = current->next; // connects the newNode to the next node in the list
            current->next = newNode; // points the current-next to the contents of the newNode pointer
        }
    }

    //Function to remove and return the character with the lowest frequency
    Node* dequeue() {
        if (!head) {
            cout << "Priority Queue is empty." << endl;
            return NULL;    //return null if the list is empty
        }
        Node* temp = head; // stores head to temp
        head = head->next; // moves the head to the next node
        temp->next=NULL; // ensures that the temp->next is not pointing to any node in the list
        return temp; // returns the value of temp
    }

    // Function to check if the priority queue is empty
    bool isEmpty()  {
        return head == NULL;
    }

    Node* buildTree() {
        while (!isEmpty()) { // loop continues as long as the list is not empty
            Node* left = dequeue(); // stores the first node to left
            if (isEmpty()) { // checks if list is empty
                // If there's only one element left, it becomes the root of the Huffman tree
                return left;
            }

            Node* right = dequeue(); // stores the second node to the right
            int freqSum = left->node->frequency + right->node->frequency; // adds the frequency of the  left and right node

            // Create a new node with the combined frequency
            NodeData* freqNodeData = new NodeData('\0', freqSum); // '\0' acts as a place holder since only the frequency is needed
            freqNodeData->left = left->node; // assigns the left node as a child of freqNodeData
            freqNodeData->right = right->node; // assigns the right node as a child of freqNodeData

            // Create a new node for the freqNode
            Node* freqNode = new Node(freqNodeData);

            // Insert the new internal node back into the priority queue based on frequencies
            if (!head || freqNode->node->frequency < head->node->frequency) { // condition checking if the frequency is less than the frequency in the head pointer
                freqNode->next = head; // connects the next pointer of freqNode to the rest of the list
                head = freqNode; // assigns freqNode as the head
            } else {
                Node* current = head; // assigns current to head for traversal
                while (current->next && freqNode->node->frequency >= current->next->node->frequency) {
                    current = current->next; // traverses the list as long as the frequency of freqNode is greater than or equal to the frequency of the current node
                }
                freqNode->next = current->next; // connext the next pointer of the freqNode to the rest of the list
                current->next = freqNode; // assigns current->next to freqNode
            }
        }
        return NULL;
    }

string convertBinaryToHex(const string& binaryString) { // function to convertBinaryToHex
    string hexResult;

    for (size_t i = 0; i < binaryString.size(); i += 8) { // iterates through chunks of 8 bits
        string byteValue = binaryString.substr(i, 8); // extracts 8bits on the binaryString
        int decimalPart = stoi(byteValue, NULL, 2); //convert the extracted byte value to a decimal

        // Convert the byte to hexadecimal
        char hexChar[3]; // hexChar stores 2 hex digit and a null terminator
        sprintf(hexChar, "%02X", decimalPart); // prints the hex in 2digit format
        hexResult += hexChar; // appends each result per iteration
    }

    return hexResult; // returns the converted value
}

// compression code that traverses the tree to get the code of each  char
void compression(NodeData* root, char character, string& result, string& charFreq, string code = "") {
    if (root != NULL) { // if root is not equal to null
        if (root->data == character) { // checks if the root->data is equal to the passed character
            result += code; // appends the code in the traversal
        }
        compression(root->right, character, result,  charFreq,code + "1"); // recursive function to traverse the right of the tree with a code 1
        compression(root->left, character, result, charFreq, code + "0"); // recursive function to traverse the left of the tree with a code 0
    }
}

void display(NodeData* root, string code, string& charFreq, char character) { // function to display the ascii, data, freq, and code
    if (root != NULL) {
        if (root->data == character) { //
            cout << (int)(root->data) << "\t" << root->data << "\t" << root->frequency << "\t\t" << code << endl;
            // Append character-frequency pairs to the string
            charFreq += root->data + to_string(root->frequency) + " ";
        }
        display(root->left, code + "0", charFreq, character);
        display(root->right, code + "1", charFreq, character);
    }
}

int calculateBytes(const string& input) { // calculates the total bytes
    int length = input.length();  // Get the number of characters in the string
    int quotient = length / 2;    // Divide the length by 2 to get the quotient
    int remainder = quotient + 1; // Calculate the remainder as the sum of the quotient and 1
    return remainder;
}

};
// function to print borders
void border(){
    cout << "\n";
    for (int i=0; i < 177; i++)
        cout << "=";
    cout << "\n";
}

int main() {
    // OPEN FILE FOR READING
    ifstream fp("D:\\StringInput.txt"); // opens the file path in read mode
    if (fp.fail()) { // error handling
        cout << "File not found";
    }
    else {
    PriorityQueue pq;
    int charFrequency[NUM_CHARACTERS] = {0}, originalLength; // array to count the frequency
    string binaryString, textInput, charFreqString, wholeString; // binaryString: Binary code, textInput: Text from file

    while(getline(fp, textInput)){ // reads the whole content of the file instead of reading a single line
        wholeString += textInput; // appends the textInput in each line to a variable wholeString
    }
    for (char ch : wholeString) { // stores frequency of each character in the text input in the array
        charFrequency[(unsigned char)(ch)]++;
    }
    for (int i = 0; i < wholeString.size(); ++i) { // enqueuing
        if (charFrequency[(unsigned char)(wholeString[i])] > 0) {
            pq.enqueue(wholeString[i], charFrequency[(unsigned char)(wholeString[i])]);
            charFrequency[(unsigned char)(wholeString[i])] = 0; // Set frequency to 0 to avoid re-enqueuing
        }
    }
    
    Node* root = pq.buildTree(); // function call to build tree
    //Node* head;
    
    // Displaying the outputs
    cout << "\nASCII\tCHAR\tFREQUENCY\tHUFFMAN CODE"<<endl;
    // compressing each character in the text file to their corresponding huffman code
    for (int i = 0; i < wholeString.size(); ++i) { // outerloop iterates through the whole lenght of wholeString
    // Check if the character has already been processed
    bool isAlreadyProcessed = false;
    for (int j = 0; j < i; ++j) { // inner loop compares if the wholeStrinb is already processed or not so that it wont be displayed duplicately
        if (wholeString[i] == wholeString[j]) {
            isAlreadyProcessed = true; // returns true if wholeString[i] is equal to wholeSring[j]
            break;
        }
    }
    // If the character is not already processed, display it
    if (!isAlreadyProcessed) {
    pq.display(root->node, "", charFreqString, wholeString[i]);
        }
    }

    for (char character : wholeString) {
        pq.compression(root->node, character, binaryString, charFreqString);
    }
    border();
    cout << "\nText Input: " << wholeString << endl ;
    border();

    cout<< "\nHuffman Code: " << binaryString << endl;
    string hexResult = pq.convertBinaryToHex(binaryString); // converting binary to hexadecimal and storing it to local variable
    border();

    cout << "\nThe Hexadecimal value is: " << hexResult<<endl;

    border();

    int totalBytes = pq.calculateBytes(hexResult);
    cout << "\nTotal Bytes of the Text String: " << wholeString.length() << endl;
    cout << "Total Bits of Huffman Code: " << binaryString.length() << endl;
    cout << "Total Compressed Bytes: " << totalBytes << endl;
    delete root; // deletes memory allocation
    fp.close(); // closing the file for reading

    // Saving the compressed version in the file
    ofstream of("compressionFile.txt"); // opening the file for writing
    if (!of.is_open()) {
        cout << "Error! File can not be opened." << endl;
        delete root;
        return 1; // Exit the program indicating an error
    }
    originalLength = binaryString.size(); // returns the size of binaryString and store it to a local variable
    // writing in the file
    of << hexResult << endl; //compressed binary to hexadecimal
    of << originalLength<< endl; // size of the binary string
    of << charFreqString; // combination of characters and frequency
    of.close(); // closes the file for writing
    }
    return 0;
}
