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
Hold* InishalizeQueue(void);
char* DeQueue (Hold* hold);
void EnQueue (Hold* hold, char book[50], char title[50]);
Book* CreateNewHold(char book[50], char name[50]);
bool AreHolds (Hold* holds);
Book* Front (Hold* hold);

//prototypes (hash table)
int GenerateHash(const char* Title);
BookRegister* InitializeHashTable(void);
BookTitleKeyValuePair* InitializeKeyValuePair(const char* Title, const char* HashKey);
void AddToHashTable(BookRegister* hashTable, const char* Title, const char* HashKey);
const char* SearchByTitleHashTable(BookRegister* hashTable, const char* word);

int main(int argc, const char* argv[]){
BookRegister* hashTable = InitializeHashTable();

}

//inishalize queue
Hold* InishalizeQueue(void){
    Hold* holds = (Hold*)malloc(sizeof(Hold));
    if(holds == NULL){
    printf("No Memory");
    exit(1);
}

Hold->Front = NULL;
Hold->Back = NULL;
return holds;
}


//dequeue
char* DeQueue (Hold* hold){
    if(AreHolds(hold)){
        printf("Queue is Empty");
        return -1;
    }
    
    Node* holdToDeQueue = hold->Front;
    char* dataToDeQueue = holdToDeQueue->holder;
    hold->Front = holdToDeQueue->NextHold; // Resetting Head
    free(holdToDeQueue); // Free Up Existing Head
    return dataToDeQueue;
    
}


//enqueue
void EnQueue (Hold* hold, char book[50], char title[50]) {
    if(hold == NULL){
    hold = InititalizeQueue();
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
    int copyIndex = 0;
    do{
    newHold->title[copyIndex] = book[copyIndex];
    copyChar = newHold->title[copyIndex];
    
    }while(copyChar != NULL)
    
    do{
    newHold->holder[copyIndex] = name[copyIndex];
    copyChar = newHold->holder[copyIndex];
    
    }while(copyChar != NULL)
    newHold->NextHold = NULL;
    return newHold;
}


//is empty
bool AreHolds (Hold* holds){
    
    return Hold->Front == NULL;
}


//front
Book* Front (Hold* hold){
    
    if(AreHolds(hold)){
        printf("Queue is Empty");
        return -1;
    }
    
    return hold->Front->holder;
    
}

//hash table===================================================================
int GenerateHash(const char* Title) {
    int hash = 0;
    for (int i = 0; Title[i] != '\0'; i++) {
        int asciiValue = Title[i];
        hash = (hash + asciiValue) % SIZE;
    }
    return hash;
}

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

void AddToHashTable(BookRegister* hashTable, const char* Title, const char* HashKey) {
    BookTitleKeyValuePair* kvp = InitializeKeyValuePair(Title, HashKey);
    int hash = GenerateHash(Title);

    if (hashTable->Table[hash] == NULL) { 
        hashTable->Table[hash] = kvp;
        return;
    }

    BookTitleKeyValuePair* current = hashTable->Table[hash];
    while (current->NextKeyValuePair != NULL) {
        current = current->NextKeyValuePair;
    }

    current->NextKeyValuePair = kvp;
}

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



