#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
//#include <time.h>
#include <ctype.h>

// define a maximum length for the string.
#define MAX_LEN (200)
// define the alphabet size for english.
#define ALPHABET_SIZE (26)

// linked String structure
typedef struct LinkedString {
    // linked string node contains a char.
    char character;
    struct LinkedString *next;

} LinkedString;

// TrieNode structure
typedef struct TrieNode {

    struct TrieNode *children[ALPHABET_SIZE];
    // linked string to place the str inside the trie node.
    LinkedString *LinkedString;
    // boolean to check for leaf_nodes
    bool is_leaf;

} TrieNode;

/* function  */

// create a trie Node(radix).
TrieNode *createNode();
// appends to the string[initial: end] and return a linked string.
LinkedString *appendLinkedString(char *str, int inital, int end);
// break the linked string and create a node with the breaked string and returns intial.
TrieNode *breakLinkedString(TrieNode *previousNode, TrieNode *node, LinkedString *breakPoint);
// insert a str to the trie node.
void insert(TrieNode *root, char *str);
// returns the pointer if found the str else returns NULL
TrieNode *searchNodes(TrieNode *root, char *str);
// convert a string in to charactors
int str_to_char(LinkedString *begin, char *str, int initial);
// print suggestions for the prefix.
void printSuggetions(TrieNode *suggestedNode, char str[], int Size);
// create a linked string with a given char.
LinkedString *createString(char Char);

// node count to get the memory used.
int node_count = 0;
// link string count to get the memory.
int link_string_node_count = 0;

int main(){

    char input[MAX_LEN];
    char str[MAX_LEN];
    char lowerCase[MAX_LEN];

    // read the file
    TrieNode *root = createNode();

    // file name of the wordlist
    char *fileName = "words.txt";

    // create a file pointer to read the file
    FILE *fptr = fopen(fileName, "r");

    int j = 0;

    // read the file and insert words into the tire
    while (fgets(str, MAX_LEN - 1, fptr))
    {
        j = 0;
        // check whether the character is in alphabet
        for (int i = 0; i < MAX_LEN; ++i){

            if (((str[i] > 64) && (str[i] < 91)) || (str[i] > 96) && (str[i] < 123)) {
                // converts the characters into lowercase
                lowerCase[j] = tolower(str[i]);
                j++;
            }
            if (str[i] == '\n'|| str[i] == '\0') {
                break;
            }
        }
        // configure the terminating character
        lowerCase[j] = 0;
        // insert into the trie
        insert(root, lowerCase);
    }


    while (true){
        // prompt the user to inputs
        printf("\n\t Enter the prefix :\n");

        // get the input
        scanf("%s", input);
        // copy the input to the str
        strcpy(str, input);

        //if the exit condition is met
        if (strcmp(str, "0") == 0){
            printf("Quit! \n");
            break;
        }

        j = 0;
        // check whether the character is in alphabet
        for (int i = 0; i < MAX_LEN; ++i){

            if (((str[i] > 64) && (str[i] < 91)) || (str[i] > 96) && (str[i] < 123)) {
                // converts the characters into lowercase
                lowerCase[j] = tolower(str[i]);
                j++;
            }
            if (str[i] == '\n' || str[i] == '\0') {
                break;
            }
        }
        // configure the terminating character
        lowerCase[j] = 0;
        strcpy(str, lowerCase);

        // check for the prefix
        TrieNode *suggestedNode = searchNodes(root, str);
        if (!suggestedNode) {
            // if prefix not found
            printf("Prefix Not Found!\n\n");
            continue;
        }

        printf("\n *****************  AUTOCOMPLETE (SUGGETIONS) ****************** \n\t");
        printSuggetions(suggestedNode, str, strlen(str));

    }

    return 0;
}

TrieNode *createNode() {

    node_count++;
    // allocate space for new trie node.
    TrieNode *newtrieNode = (TrieNode *)malloc(sizeof(TrieNode));
    // initialize the linked string with NULL
    newtrieNode->LinkedString = NULL;
    // make it as not a leaf
    newtrieNode->is_leaf = false;

    int i;
    for (i = 0; i < ALPHABET_SIZE; ++i) {
        // make all the chiledrens null.
        newtrieNode->children[i] = NULL;
    }
    return newtrieNode;
}

LinkedString *createString(char Char) {

    link_string_node_count++;
    // allocate space for a new string dynamically.
    LinkedString *newString = (LinkedString *)malloc(sizeof(LinkedString));
    // put the char into its character
    newString->character = Char;
    // make the next pointer null.
    newString->next = NULL;
    // return the new string.
    return newString;
}
// appends to the string[initial: end] and return a linked string.
LinkedString *appendLinkedString(char *str, int inital, int end) {
    int i;
    // create a pointers to linked strings.
    LinkedString *current = createString(str[inital]);
    LinkedString *newString = NULL;
    LinkedString *string = current;

    // go from initial position to end position
    for (i = inital + 1; i < end; ++i) {
        // crate a new string with char index i or str
        newString = createString(str[i]);
        // make the next pointer point to new string.
        current->next = newString;
        // go through the string
        current = current->next;
    }
    // make the last pointer at the end.
    current = NULL;
    return string;
}

TrieNode *breakLinkedString(TrieNode *previousNode, TrieNode *node, LinkedString *breakPoint) {
    // create a new trie node pointer.
    TrieNode *newNode = createNode();
    int mdash;
    // create a new string beginning next to the break point.
    LinkedString *newString = breakPoint->next;
    breakPoint->next = NULL;
    // convert char to index
    int index1 = (newString->character) &mdash; 'a';

    newNode->LinkedString = node->LinkedString;
    node->LinkedString = newString;
    newNode->children[index1] = node;

    int index2 = (newNode->LinkedString->character) - 'a';
    // pointer the new node to the relevant index of the parent node.
    previousNode->children[index2] = newNode;

    // return the newnode.
    return newNode;
}

void insert(TrieNode *root, char *str) {

    // get the length of the str
    int lastLetterIndex = strlen(str);
    int i = 0, charIndex;

    // create trie nodes and linked list pointers to track
    // previous current and next pointer nodes.
    TrieNode *currentNode = root, *previousNode = NULL;
    TrieNode *newNode = NULL;
    LinkedString *currentLetter, *previousLetter = NULL;
    currentLetter = currentNode -> LinkedString;

    // go till the last letter of the string.
    while (i < lastLetterIndex) {

        charIndex = (str[i]) - 'a';

        // if current letter is null
        if (currentLetter == NULL) {

            // if the trie is empty.
            if (currentNode->children[charIndex] == NULL) {
                // create a new node.
                newNode = createNode();
                // append the string[i:lastLetterIndex] point to the new nodes linked
                newNode->LinkedString = appendLinkedString(str, i, lastLetterIndex);
                newNode->is_leaf = true;
                currentNode->children[charIndex] = newNode;
                break;

            } else { // if it is the first node.
                // make the previous node pointing to the current node.
                previousNode = currentNode;
                currentNode = currentNode->children[charIndex];
                previousLetter = currentNode->LinkedString;
                currentLetter = currentNode->LinkedString->next;
            }

        } else {

            if (currentLetter->character != str[i]) {
                // make the current node pointing to the breakedLinkedString.
                currentNode = breakLinkedString(previousNode, currentNode, previousLetter);
                // create a new node.
                newNode = createNode();
                // append the link string to the newNodes LinkedString.
                newNode->LinkedString = appendLinkedString(str, i, lastLetterIndex);
                // make new node a leaf node
                newNode->is_leaf = true;
                // put newNode as a child of a current node.
                currentNode->children[charIndex] = newNode;
                break;

            } else {
                previousLetter = currentLetter;
                currentLetter = currentLetter->next;
            }
        }
        i++;
    }
}


TrieNode *searchNodes(TrieNode *root, char *str) {
    // get the lastIndex of the str
    int lastIndex = strlen(str);
    int i = 0, charIndex;
    TrieNode *currentNode = root;
    LinkedString *currentLetter = currentNode->LinkedString;

    while (i < lastIndex) {

        // get the index from the char Index.
        charIndex = str[i] - 'a';

        // if current letter is null.
        if (currentLetter == NULL) {

            // point the relevant index to the currentNode.
            currentNode = currentNode->children[charIndex];
            // if the word is not found. Then the current node
            // and the currentLetter is NULL.
            if (!currentNode) return NULL;
            // make the currentLetter pointing to the linked strings next letter.
            currentLetter = currentNode->LinkedString->next;

        } else {
            // go to the next letter.
            currentLetter = currentLetter->next;
        }
        i++;
    }

    // go untill the current letter is found null
    while (currentLetter != NULL) {
        // go till the end of the character.
        str[lastIndex] = currentLetter->character;
        currentLetter = currentLetter->next;
        lastIndex++;
    }
    // make the last index pointing to the terminating character
    str[lastIndex] = '\0';
    return currentNode;
}

int str_to_char(LinkedString *begin, char *str, int initial) {

    int newSize = initial;
    LinkedString *currentLetter = begin;

    // go while current lettter is not null
    while (currentLetter != NULL) {
        // put the char by char in str
        str[newSize] = currentLetter->character;
        // goto the next letter
        currentLetter = currentLetter->next;
        newSize++;
    }
    // set the terminating character.
    str[newSize] = '\0';
    return newSize - 1;
}

void printSuggetions(TrieNode *suggestedNode, char str[], int Size) {

    // create a poiter to the suggested node
    TrieNode *currentNode = suggestedNode;
    int i, j, newSize;

    if (currentNode->is_leaf){
        // go till the word_size
        for (j = 0; j < Size; ++j){
            // print the uppercased char
            printf("%c", toupper(str[j]));
        }
        // print the endline
        printf("\n\t");
    }

    // go through each children node.
    for (i = 0; i < ALPHABET_SIZE; ++i) {

        // if each child is not null
        if (currentNode->children[i] != NULL) {
            // break down the string and get the characters
            newSize = str_to_char(currentNode->children[i]->LinkedString, str, Size);
            // print the suggestion.
            printSuggetions(currentNode->children[i], str, newSize + 1);
        }
    }
}
