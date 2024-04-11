/*
JHONDEL MICO N. ABAS
CHRISTIAN DAYNE R. MENDERICO
BSIT-2A
FILE DECOMPRESSION
*/

//Standard library headers for the code
#include <iostream>
#include <fstream>
#include <string>

#define MAX 1000

using namespace std;

struct NodeData { //Structure for creating the nodes for the characters and their corresponding frequencies.
    char data; //Stores the characters
    int freq; //stores the frequencies
    NodeData* left;
    NodeData* right;

    NodeData(char c = '\0', int freq = 0) : data(c), freq(freq), left(nullptr), right(nullptr) {}
};

struct Node { //Structure for creating the tree appending NodeData
    NodeData *node;
    Node* next;
    Node(NodeData* bn): node(bn), next(NULL){}
};

class PriorityQueue {
private:
    Node* head; //To access the head of the Node list
    string huffmanCodes[MAX]; //Array that stores all of the huffman codes of each 8 bits
public:
    PriorityQueue() : head(NULL) {} //initializes head to NULL

void enqueue(NodeData* bn) { //Function to enqueue each characters and their corresponding freq to the Huffman Tree
        Node* newNode = new Node(bn); //Creates a new node for the node bn that contains the charac and freq

        if (!head || newNode->node->freq < head->node->freq) { //Appends the newNode to the head if its the first element or if its freq is less than the first element
            newNode->next = head;
            head = newNode;
        } else {
            Node* current = head; //Initializes current to head
            while (current->next && newNode->node->freq >= current->next->node->freq) { //Traverses until the frequency of newNode is higher than the current Node.
                current = current->next;
            }
            newNode->next = current->next; //Appends the newNode after the current node
            current->next = newNode; //assigns the newNode to the current pointer
        }
}

NodeData* dequeue() { //Function to dequeue the nodes from the list when it is enqueued to the tree
            if(head == NULL) { //Condition if there is nothing in the list
                return NULL;
            } else { //Deletes the node in the head of the list and re-assigns the head to head->next
                Node* temp = head;
                head = head->next;
                NodeData* bn = temp->node;
                delete temp;
                return bn;
            }
}

void printData(NodeData* charac, int size) { //Function for creating and printing the huffman codes for each character
    int count = 0;

    for (int i = 0; i <size; i++) { //enqueues the characters and frequencies inside array charac
                enqueue(&charac[i]);
                count++; //To track the number of indexes enqueued
            }

    while (count > 1) { //While loop to extract the root of the huffman tree, it will stop until the Node only has 1 data which is the root
                //Dequeues the first two nodes in the list
                NodeData* left = dequeue();
                NodeData* right = dequeue();
                //Creates newNode that adds the left and right node
                NodeData* newNode = new NodeData(' ', left->freq + right->freq);
                newNode->left = left;
                newNode->right = right;
                enqueue(newNode); //enqueues the new node to the list to the list to build the tree
                count--; //subtracts the count since one node is dequeued
    }

huffmanCode(head->node, ""); //Function to create the huffman codes by traversing through the tree

cout << "ASCII\tCharacter\tFrequency\tHuffman Code" << endl; //prints the characters, frequencies, and their corresponding huffman codes
    for (int i = 0; i < size; ++i) {
        int ascii = (charac[i].data);
        cout << ascii << "\t" << charac[i].data << "\t\t" << charac[i].freq << "\t\t" << huffmanCodes[ascii] << endl;
    }
}

void huffmanCode(NodeData* root, string code) { //Function to create the huffman codes of the characters
        if (root->left == NULL && root->right == NULL) { //Stops the recursion if it reaches a leaf node
            huffmanCodes[root->data] = code; //Stores the huffman code built to the huffmanCode array
            return;
        }
        huffmanCode(root->left, code + "0"); //Recursive function for reading the tree and building the code
        huffmanCode(root->right, code + "1"); //Recursive function for reading the tree and building the code
}

Node* getNode() const { //Getter for the head of the node
        return head;
    }

string decode(NodeData* root, string str){ //Function to decode the huffman code of the text file using the tree
    string out = "";

    struct NodeData* curr = root;
    for (int i=0;i<str.size();i++){
       if (str[i] == '0'){
           //if current bit is 0 move to the left node.
           curr = curr->left;
       }else{
           //if current bit is 1 move to the right node.
           curr = curr->right;
       }

        //leaf node is encountered
        if (curr->left==NULL && curr->right==NULL){
            out += curr->data;
            curr = root;
        }
    }
    return out+'\0';
}

string hexToBinary(const string& hexString) { //Function to convert hex values inside the compressed file
    //Map as basis for the values of hexadecimal characters
    const char* hexToBinaryMap = "0000""0001""0010""0011""0100""0101""0110""0111"
                                 "1000""1001""1010""1011""1100""1101""1110""1111";

    string binaryRepresentation;

    for (char hexDigit : hexString) { //Scans the hex per digit in the hexString and checks its value
            int index = (hexDigit >= '0' && hexDigit <= '9') ? hexDigit - '0'
                        : (hexDigit >= 'A' && hexDigit <= 'F') ? hexDigit - 'A' + 10
                        : hexDigit - 'a' + 10;

            binaryRepresentation += string(hexToBinaryMap + (index * 4), 4);
    }

    // Handle the case when the input is "0"
    if (binaryRepresentation.empty()) {
        binaryRepresentation = "0";
    }

    return binaryRepresentation; //Returns the binarystring of the hex
}

};
int main() {
    PriorityQueue q; //To access the priority queue class
    string line, binaryString, currentDecimal, binaryLength, lastByte, treeRecon, decimal; //Variable Declarations
    int size = 0; //Initializes size to 0;
    NodeData charac[MAX]; //To access the NodeData node.

    ifstream of("compressionFile.txt"); //Reads compressed file
    if (!of.is_open()) {
        cout << "Error opening the file!" << endl;
    }

    getline(of, line); //Reads first line (hexCodes/HuffmanCodes) and stores it in Line
    getline(of, binaryLength); //Reads second line (length) and stores it in binaryLength
    getline(of, treeRecon); //Reads 3rd line and stores it in treeRecon
        int i=-1;
        for (char ch : line) { //traverses each character until the end of line
            currentDecimal += ch; //Appends the characters to form a 1 byte hexadecimal
        if (i%2==0) { //Check if last byte already
                    if (line[i+3]=='\0'){ //checks if the hexadecimal is the last byte
                        break;
                    }
                binaryString += q.hexToBinary(currentDecimal); //Converts the hexadecimal to binary and adds it to the binary string.

                currentDecimal.clear(); //Clears the current decimal for next iteration/
        }
        i++;
        }

        int length = stoi(binaryLength); //Converts length to int
        length = length%8; // counts the padding of the last byte

        lastByte = q.hexToBinary(currentDecimal); //Converts the last byte to binary

        if(length!=0){ //If lastByte needs padding adjustment
            if(lastByte.length()<length){ // Adds the padding for the last byte if needed
                while(lastByte.length()<length)
                lastByte = '0' + lastByte; //Adds Zeroes
            }
            if(lastByte.length()>length){ // Subtracts the padding for the last byte if needed
                while(lastByte.length()>length)
                lastByte = lastByte.substr(1);; //Subtracts Zeroes
        }
        }
        binaryString += lastByte; //Combines the Binary string and the lastByte


i = 0;
while (i < treeRecon.length()) { //Traverses through the line
    if (i == 0){ //If first character, consider it as character
        charac[size].data = treeRecon[i]; //append to the charac array
        i++;
        while (treeRecon[i] != ' ' && treeRecon[i] != '\0') { //adds the frequency
            decimal += treeRecon[i]; //combines the charac to form the freq
            i++;
        }
        charac[size].freq = stoi(decimal); //appends the frequency to the charac array
        decimal.clear(); //clears the decimal variable
        size++; //updates the index of the charac node
    }
    if (treeRecon[i-1] == ' ') { //Checks if the charac has a space before
        charac[size].data = treeRecon[i]; //Stores the character as character
        i++;
        while (treeRecon[i] != ' ' && treeRecon[i] != '\0') { //adds the frequency
            decimal += treeRecon[i]; //combines the charac to form the freq
            i++;
        }
        charac[size].freq = stoi(decimal); //appends the frequency to the charac array
        decimal.clear(); //clears the decimal placeholder
        size++; //updates the index of the charac node
    }
    i++; //updates the index of the string array treeRecon
}

    q.printData(charac, size); //For building the charac node and printing the huffman table

    Node* head = q.getNode(); //Gets the head of the huffman tree

    string decodedString = q.decode(head->node, binaryString); //Function for decoding the messsage

    //Formatting codes
    cout << endl;
    for (i = 0; i < 237; i++){
        cout << "=";
    }
    cout << endl << "Binary Representation: " << binaryString << endl << endl; // Prints the Binary String
    for (i = 0; i < 237; i++){
        cout << "=";
    }
    cout << endl << "Decoded Huffman Data: " << decodedString << endl << endl; // Prints the Decoded Message

    of.close();

return 0;
}
