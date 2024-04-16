#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Book {
    char holder[50];        //library user requesting a book hold
    char title[50];         //book title
    struct Book* NextHold;  //next user hold in queue
} Book;

struct Hold{//Queue
    Book* Front;    //head pointer
    Book* Back;     //tail pointer
} Hold;

//structs for hash table
typedef struct BookTitleKeyValuePair {
    char* Title;
    char* HashKey;
    struct BookTitleKeyValuePair* NextKeyValuePair;
} BookTitleKeyValuePair;

typedef struct BookRegister {
    BookTitleKeyValuePair* Table[SIZE];
} BookRegister;


//prototypes (queue)
Hold* InitializeQueue(void);
char* DeQueue (Hold* hold);
void EnQueue (Hold* hold, char book[50], char title[50]);
Book* CreateNewHold(char book[50], char name[50]);
bool AreHolds (Hold* holds);
char* Front (Hold* hold);

//prototypes (hash table)
int GenerateHash(const char* Title);
BookRegister* InitializeHashTable(void);
BookTitleKeyValuePair* InitializeKeyValuePair(const char* Title, const char* HashKey);
void AddToHashTable(BookRegister* hashTable, const char* Title, const char* HashKey);
const char* SearchByTitleHashTable(BookRegister* hashTable, const char* word);

int main(int argc, const char* argv[]){
BookRegister* hashTable = InitializeHashTable();

}


//Queue===================================================================

//inishalize queue
Hold* InitializeQueue(void){
    Hold* holds = (Hold*)malloc(sizeof(Hold));
    if(holds == NULL){
        printf("No Memory");
        exit(1);
    }

    holds->Front = NULL;
    holds->Back = NULL;
    return holds;
}


//dequeue

char* DeQueue (Hold* hold){
    if(AreHolds(hold) == false){
        printf("Queue is Empty");
        return '\0';
    }
    
    Book* holdToDeQueue = hold->Front;
    char* dataToDeQueue = holdToDeQueue->holder;
    hold->Front = holdToDeQueue->NextHold; // Resetting Head
    free(holdToDeQueue); // Free Up Existing Head
    return dataToDeQueue;
    
}


//enqueue
void EnQueue (Hold* hold, char book[50], char title[50]) {
    if(hold == NULL){
        hold = InitializeQueue();
    }

    Book* toEnqueue = CreateNewHold(book, title);
    if (AreHolds(hold)) {
        hold->Front = toEnqueue;
        hold->Back = toEnqueue;
    } 
    else {
        hold->Back->NextHold = toEnqueue;
        hold->Back = toEnqueue;
    }
}


//new node
Book* CreateNewHold(char book[50], char name[50]){
    Book* newHold = (Book*)malloc(sizeof(Book));
    if(newHold == NULL){
        printf("No Memory");
        exit(1);
    }
    char copyChar = '\0';
    for(int i; i < 50; i++){
        newHold->title[i] = book[i];
    }
    
    for(int i; i < 50; i++){
        newHold->holder[i] = name[i];
    }
    
    newHold->NextHold = NULL;
    return newHold;
}


//is empty
bool AreHolds (Hold* holds){
    return holds->Front == NULL;
}


//front
char* Front (Hold* hold){
    
    if(AreHolds(hold)){
        printf("Queue is Empty");
        return NULL;
    }
    
    return hold->Front->holder;
}

//hash table===================================================================
//create the hash table
int GenerateHash(const char* Title) {
    int hash = 0;
    for (int i = 0; Title[i] != '\0'; i++) {
        int asciiValue = Title[i];
        hash = (hash + asciiValue) % SIZE;
    }
    return hash;
}

//initialize the hash table
BookRegister* InitializeHashTable(void) {
    BookRegister* hashTable = (BookRegister*)malloc(sizeof(BookRegister));
    if (hashTable == NULL) {
        printf("End of memory");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < SIZE; i++) {
        hashTable->Table[i] = NULL;
    }

    return hashTable;
}

//linking the key to the value
BookTitleKeyValuePair* InitializeKeyValuePair(const char* Title, const char* HashKey) {
    BookTitleKeyValuePair* kvp = (BookTitleKeyValuePair*)malloc(sizeof(BookTitleKeyValuePair));
    if (kvp == NULL) {
        printf("EOM");
        exit(EXIT_FAILURE);
    }

    kvp->Title = _strdup(Title);
    kvp->HashKey = _strdup(HashKey);
    kvp->NextKeyValuePair = NULL;
    return kvp;
}

//Add to hash table - seperate chaining method
void AddToHashTable(BookRegister* hashTable, const char* Title, const char* HashKey) {
    BookTitleKeyValuePair* kvp = InitializeKeyValuePair(Title, HashKey);
    int hash = GenerateHash(Title);

    if (hashTable->Table[hash] == NULL) { //bucket is empty
        hashTable->Table[hash] = kvp;
        return;
    }

    //collision occured - create array
    BookTitleKeyValuePair* current = hashTable->Table[hash];
    while (current->NextKeyValuePair != NULL) {
        current = current->NextKeyValuePair;
    }

    current->NextKeyValuePair = kvp;
}

//Search hash table - seperate chaining method
const char* SearchByTitleHashTable(BookRegister* hashTable, const char* word) {
    int hash = GenerateHash(word);

    if (hashTable->Table[hash] == NULL) {
        return "ERROR: cannot find the Key in the table.";
    }

    BookTitleKeyValuePair* current = hashTable->Table[hash];

    while (current != NULL) {
        if (strcmp(current->Title, word) == 0) {
            return current->HashKey;
        }
        current = current->NextKeyValuePair;
    }

    return "ERROR: cannot find the Key in the table.";
}



