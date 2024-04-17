#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define SIZE 20
#pragma warning(disable: 4996)

typedef struct Book {
    char holder[50];
    char title[50];
    char author[50];
    struct Book* NextHold;
} Book;

typedef struct Hold {
    Book* Front;
    Book* Back;
} Hold;

typedef struct BookTitleKeyValuePair {
    int BookID;
    char* Title;
    char* Author;
    struct BookTitleKeyValuePair* NextKeyValuePair;
} BookTitleKeyValuePair;

typedef struct BookRegister {
    BookTitleKeyValuePair* Table[SIZE];
} BookRegister;

typedef struct BookStackNode {
    char title[50];
    char author[50];
    struct BookStackNode* next;
} BookStackNode;

typedef struct {
    BookStackNode* top;
} BookStack;

typedef struct BSTNode {
    int bookID;
    char title[50];
    char author[50];
    struct BSTNode* left, * right;
} BSTNode;

//prototypes (queue)
Hold* InitializeQueue(void);
char* DeQueue (Hold* hold);
void EnQueue (Hold* hold, char book[50], char title[50]);
bool AreHolds(Hold* holds);
Book* CreateNewHold(char book[50], char name[50]);
char* Front (Hold* hold);

//prototypes (hash table)
int GenerateHash(int bookID);
BookRegister* InitializeHashTable(void);
BookTitleKeyValuePair* InitializeKeyValuePair(int ID, const char* title, const char* author);
void AddToHashTable(BookRegister* hashTable, int bookID, const char* hashKey, const char* author);
BookTitleKeyValuePair* SearchByBookIDHashTable(BookRegister* hashTable, int bookID);
BookTitleKeyValuePair* SearchByTitle(BookRegister* hashTable, const char* title);

//prototypes (stack)
BookStack* InitializeStack(void);
void Push(BookStack* stack, char title[50]);
char* Pop(BookStack* stack);

//prototypes (Binary Search tree)
BSTNode* NewBSTNode(int id, char title[50], char author[50]);
BSTNode* Insert(BSTNode* node, int bookID, char title[50], char author[50]);
void Inorder(BSTNode* root);

//function prototypes
void DisplayMenu();
void HandleSearchBook(BookRegister* hashTable);
void HandleBorrowBook(BookStack* stack, Hold* queue, BookRegister* hashTable);
void HandleReturnBook(BookRegister* hashTable);
void HandleDisplayBST(BSTNode* root);
bool IsBookAvailable(BookRegister* hashTable, const char* title);

int main(void) {
    BookRegister* hashTable = InitializeHashTable();
    Hold* queue = InitializeQueue();
    BookStack* stack = InitializeStack();
    BSTNode* bstRoot = NULL;

    // Book details for initializing both BST and Hash Table
    int bookIDs[] = { 1011, 2521, 3035, 4048, 9520, 3020, 1001, 1201 };
    char* titles[] = {
        "Surrounded by Idiots", "How to win friends and influence people",
        "Power of Your Subconcious Mind", "Thinking Fast and Slow",
        "Rich Dad, Poor Dad", "The 7 Habits of Highly Effective People",
        "Data structures and algorithms in C++", "Clean Code"
    };
    char* authors[] = {
        "Thomas Erikson", "Dale Carnegie", "Joseph Murphy", "Daniel Kahneman",
        "Robert Kiyosaki", "Stephen Covey", "Adam Drozdek", "Robert Cecil Martin"
    };

    int numBooks = sizeof(bookIDs) / sizeof(bookIDs[0]);
    for (int i = 0; i < numBooks; i++) {
        // Insert book into BST
        bstRoot = Insert(bstRoot, bookIDs[i], titles[i], authors[i]);
        // Add the same book to the hash table
        AddToHashTable(hashTable, bookIDs[i], titles[i], authors[i]);
    }

    while (1) {
        DisplayMenu();
        int choice;
        printf("Enter your choice: ");
        scanf("%d", &choice);
        while (getchar() != '\n');

        switch (choice) {
        case 1:
            HandleSearchBook(hashTable);
            break;
        case 2:
            HandleBorrowBook(stack, queue, hashTable);
            break;
        case 3:
            HandleReturnBook(hashTable);
            break;
        case 4:
            HandleDisplayBST(bstRoot);
            break;
        case 5:
            printf("Exiting the program.\n");
            return 0;
        default:
            printf("Invalid choice, please try again.\n");
        }
    }
    return 0;
}

void DisplayMenu() {
    printf("\n--- Book Management System Menu ---\n");
    printf("1. Search Book by BookId\n");
    printf("2. Borrow Book\n");
    printf("3. Return Book\n");
    printf("4. Display Books\n");
    printf("5. Exit\n");
}

void HandleSearchBook(BookRegister* hashTable) {
    int bookID;
    printf("Enter book ID to search: ");
    scanf("%d", &bookID);

    BookTitleKeyValuePair* result = SearchByBookIDHashTable(hashTable, bookID);
    if (result == NULL) {
        printf("ERROR: cannot find the book with ID %d in the table.\n", bookID);
    }
    else {
        printf("\nBook found with following Information: \n");
        printf("Title: %s\n", result->Title);
        printf("Author: %s\n", result->Author);
    }
}

void HandleBorrowBook(BookStack* stack, Hold* queue, BookRegister* hashTable) {
    int bookID;
    printf("Enter Book ID of the book to borrow: ");
    scanf("%d", &bookID);

    BookTitleKeyValuePair* book = SearchByBookIDHashTable(hashTable, bookID);
    if (book == NULL) {
        printf("Book not found in the library.\n");
    } else {
            Push(stack, book->Title);
            EnQueue(queue, "Holder Name", book->Title);  // Assuming "Holder Name" is a placeholder
            printf("Book '%s' borrowed successfully.\n", book->Title);
        }
}

bool IsBookAvailable(BookRegister* hashTable, const char* title) {
    for (int i = 0; i < SIZE; i++) {
        BookTitleKeyValuePair* current = hashTable->Table[i];
        while (current != NULL) {
            if (strcmp(current->Title, title) == 0) {
                return true; // Book found
            }
            current = current->NextKeyValuePair;
        }
    }
    return false; // Book not found
}

BookTitleKeyValuePair* SearchByTitle(BookRegister* hashTable, const char* title) {
    for (int i = 0; i < SIZE; i++) {
        BookTitleKeyValuePair* current = hashTable->Table[i];
        while (current) {
            if (strcmp(current->Title, title) == 0) {
                return current;
            }
            current = current->NextKeyValuePair;
        }
    }
    return NULL;  // Book not found
}

void HandleReturnBook(BookRegister* hashTable) {

    int bookID;
    printf("Enter the Book ID of the book you want to return: ");
    scanf("%d", &bookID);

    BookTitleKeyValuePair* book = SearchByBookIDHashTable(hashTable, bookID);
    if (book == NULL) {
        printf("No such book found in the library.\n");
    }
    else {
        printf("Book '%s' returned successfully.\n", book->Title);
    }
}

void HandleDisplayBST(BSTNode* root) {
    printf("\nAll the books in Library:\n");
    Inorder(root);
}

//Queue===================================================================
bool AreHolds(Hold * holds) {
        return holds->Front == NULL;
    }


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

Book* CreateNewHold(char book[50], char name[50]){
    Book* newHold = (Book*)malloc(sizeof(Book));
    if(newHold == NULL){
        printf("No Memory");
        exit(1);
    }
    char copyChar = '\0';
    for(int i = 0; i < 50; i++){
        newHold->title[i] = book[i];
    }
    for(int i = 0; i < 50; i++){
        newHold->holder[i] = name[i];
    }
    newHold->NextHold = NULL;
    return newHold;
}

char* Front (Hold* hold){
   
    if(hold->Front == NULL){
        printf("Queue is Empty");
        return NULL;
    }
    return hold->Front->holder;
}

int GenerateHash(int bookID) {
    return bookID % SIZE;
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

BookTitleKeyValuePair* InitializeKeyValuePair(int ID, const char* title, const char* author) {
    BookTitleKeyValuePair* kvp = (BookTitleKeyValuePair*)malloc(sizeof(BookTitleKeyValuePair));
    if (kvp == NULL) {
        printf("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    kvp->BookID = ID;
    kvp->Title = strdup(title); 
    kvp->Author = strdup(author); 
    kvp->NextKeyValuePair = NULL;
    return kvp;
}

//Add to hash table - seperate chaining method
void AddToHashTable(BookRegister* hashTable, int bookID, const char* title, const char* author) {
    int index = GenerateHash(bookID);
    BookTitleKeyValuePair* kvp = (BookTitleKeyValuePair*)malloc(sizeof(BookTitleKeyValuePair));
    if (kvp == NULL) {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    kvp->BookID = bookID;
    kvp->Title = strdup(title);
    kvp->Author = strdup(author);
    kvp->NextKeyValuePair = NULL;

    if (hashTable->Table[index] == NULL) {
        hashTable->Table[index] = kvp;
    }
    else {
        BookTitleKeyValuePair* current = hashTable->Table[index];
        while (current->NextKeyValuePair) {
            current = current->NextKeyValuePair;
        }
        current->NextKeyValuePair = kvp;
    }
}

//Search hash table - seperate chaining method
BookTitleKeyValuePair* SearchByBookIDHashTable(BookRegister* hashTable, int bookID) {
    int index = GenerateHash(bookID);
    BookTitleKeyValuePair* current = hashTable->Table[index];
    while (current) {
        if (current->BookID == bookID) {
            return current;
        }
        current = current->NextKeyValuePair;
    }
    return NULL;
}

BookStack* InitializeStack(void) {
    BookStack* stack = (BookStack*)malloc(sizeof(BookStack));
    if (stack == NULL) {
        printf("Memory error");
        exit(1);
    }
    stack->top = NULL;
    return stack;
}

void Push(BookStack* stack, char title[50]) {
    BookStackNode* node = (BookStackNode*)malloc(sizeof(BookStackNode));
    if (node == NULL) {
        printf("Memory error");
        exit(1);
    }
    strcpy(node->title, title);
    node->next = stack->top;
    stack->top = node;
}

char* Pop(BookStack* stack) {
    if (stack->top == NULL) {
        return NULL;
    }
    BookStackNode* node = stack->top;
    stack->top = node->next;
    char* title = strdup(node->title);
    free(node);
    return title;
}

BSTNode* NewBSTNode(int id, char title[50], char author[50]) {
    BSTNode* node = (BSTNode*)malloc(sizeof(BSTNode));
    if (node == NULL) {
        printf("Memory error");
        exit(1);
    }
    node->bookID = id;
    strcpy(node->title, title);
    strcpy(node->author, author);
    node->left = node->right = NULL;
    return node;
}

BSTNode* Insert(BSTNode* node, int bookID, char title[50], char author[50]) {
    if (node == NULL) {
        return NewBSTNode(bookID, title, author);
    }
    if (bookID < node->bookID) {
        node->left = Insert(node->left, bookID, title, author);
    }
    else if (bookID > node->bookID) {
        node->right = Insert(node->right, bookID, title, author);
    }
    return node;
}

void Inorder(BSTNode* root) {
    if (root != NULL)
    {
        Inorder(root->left);
        printf("Book ID: %d\n", root->bookID);
        printf("Book Name: %s\n", root->title);
        printf("Author Name: %s\n\n", root->author);
        Inorder(root->right);
    }
}
