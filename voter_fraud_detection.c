#include <stdio.h>
#include <stdlib.h>

#define SIZE 10
#define ADMIN_PASSWORD 1234

/* Node for the linked list used in chaining */
typedef struct Node {
    int voterID;
    struct Node *next;
} Node;

/* Hash table: each index stores the head of a linked list */
Node *hashTable[SIZE];

/* Global counters */
int totalVotes = 0;
int fraudAttempts = 0;
int maxVoters;

/*--------------------------------------------------
   Hash Function
   Maps a Voter ID to an index from 0 to SIZE-1
--------------------------------------------------*/
int hashFunction(int voterID) {
    return voterID % SIZE;
}

/*--------------------------------------------------
   Search for a Voter ID
   Returns the node if found, otherwise NULL
--------------------------------------------------*/
Node *searchVoter(int voterID) {
    int index = hashFunction(voterID);
    Node *temp = hashTable[index];

    while (temp != NULL) {
        if (temp->voterID == voterID) {
            return temp;
        }
        temp = temp->next;
    }

    return NULL;
}

/*--------------------------------------------------
   Insert a new Voter ID using linked-list chaining
--------------------------------------------------*/
void insertVoter(int voterID) {
    int index = hashFunction(voterID);

    Node *newNode = (Node *)malloc(sizeof(Node));

    if (newNode == NULL) {
        printf("\nMemory allocation failed.\n");
        return;
    }

    newNode->voterID = voterID;

    /* Insert at the beginning of the linked list */
    newNode->next = hashTable[index];
    hashTable[index] = newNode;

    totalVotes++;

    printf("\nVote recorded successfully!\n");
}

/*--------------------------------------------------
   Delete a Voter ID
--------------------------------------------------*/
void deleteVoter(int voterID) {
    int index = hashFunction(voterID);

    Node *temp = hashTable[index];
    Node *prev = NULL;

    while (temp != NULL) {

        if (temp->voterID == voterID) {

            /* If node is the first node */
            if (prev == NULL) {
                hashTable[index] = temp->next;
            }
            else {
                prev->next = temp->next;
            }

            free(temp);

            totalVotes--;

            printf("\nVoter ID %d deleted successfully.\n", voterID);
            return;
        }

        prev = temp;
        temp = temp->next;
    }

    printf("\nVoter ID %d not found.\n", voterID);
}

/*--------------------------------------------------
   Display the complete Hash Table
--------------------------------------------------*/
void displayHashTable() {
    int i;

    printf("\n========== HASH TABLE (CHAINING) ==========\n");

    for (i = 0; i < SIZE; i++) {

        printf("Index %d: ", i);

        Node *temp = hashTable[i];

        if (temp == NULL) {
            printf("Empty");
        }
        else {
            while (temp != NULL) {
                printf("%d -> ", temp->voterID);
                temp = temp->next;
            }

            printf("NULL");
        }

        printf("\n");
    }

    printf("===========================================\n");
}

/*--------------------------------------------------
   Display Voting Statistics
--------------------------------------------------*/
void displayStatistics() {

    printf("\n========== VOTING STATISTICS ==========\n");
    printf("Maximum Voters Allowed : %d\n", maxVoters);
    printf("Total Valid Votes      : %d\n", totalVotes);
    printf("Fraud Attempts         : %d\n", fraudAttempts);
    printf("Remaining Vote Slots   : %d\n",
           maxVoters - totalVotes);
    printf("=======================================\n");
}

/*--------------------------------------------------
   Admin Authentication
--------------------------------------------------*/
int adminLogin() {
    int password;

    printf("\nEnter Admin Password: ");
    scanf("%d", &password);

    if (password == ADMIN_PASSWORD) {
        printf("Admin authentication successful.\n");
        return 1;
    }

    printf("Access Denied! Incorrect password.\n");
    return 0;
}

/*--------------------------------------------------
   Free all dynamically allocated memory
--------------------------------------------------*/
void freeAll() {
    int i;

    for (i = 0; i < SIZE; i++) {

        Node *temp = hashTable[i];

        while (temp != NULL) {

            Node *next = temp->next;
            free(temp);
            temp = next;
        }

        hashTable[i] = NULL;
    }
}

/*--------------------------------------------------
   Main Function
--------------------------------------------------*/
int main() {

    int choice;
    int voterID;
    int i;

    /* Initialize all hash table indexes */
    for (i = 0; i < SIZE; i++) {
        hashTable[i] = NULL;
    }

    printf("============================================\n");
    printf("     VOTER FRAUD DETECTION SYSTEM\n");
    printf("        USING HASHING & CHAINING\n");
    printf("============================================\n");

    /* Ask the user for voter limit */
    do {
        printf("\nEnter maximum number of voters allowed: ");
        scanf("%d", &maxVoters);

        if (maxVoters <= 0) {
            printf("Please enter a valid positive number.\n");
        }

    } while (maxVoters <= 0);

    /* Menu-driven system */
    while (1) {

        printf("\n--------------------------------------------\n");
        printf("        VOTER FRAUD DETECTION SYSTEM\n");
        printf("--------------------------------------------\n");

        printf("1. Cast Vote\n");
        printf("2. Search Voter\n");
        printf("3. Display Hash Table (Admin)\n");
        printf("4. Delete Voter (Admin)\n");
        printf("5. Display Voting Statistics\n");
        printf("6. Exit\n");

        printf("--------------------------------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {

            /*----------------------------------
               CASE 1: CAST VOTE
            ----------------------------------*/
            case 1:

                /* Check voting limit */
                if (totalVotes >= maxVoters) {
                    printf("\nVoting limit reached!\n");
                    printf("No more votes can be recorded.\n");
                    break;
                }

                printf("\nEnter Voter ID: ");
                scanf("%d", &voterID);

                if (voterID <= 0) {
                    printf("\nInvalid Voter ID.\n");
                    break;
                }

                /* Check whether voter already exists */
                if (searchVoter(voterID) != NULL) {

                    fraudAttempts++;

                    printf("\n*** FRAUD ATTEMPT DETECTED ***\n");
                    printf("Voter ID %d has already voted.\n",
                           voterID);
                    printf("Duplicate vote rejected.\n");
                }
                else {

                    /* New voter: record the vote */
                    insertVoter(voterID);
                }

                break;

            /*----------------------------------
               CASE 2: SEARCH VOTER
            ----------------------------------*/
            case 2:

                printf("\nEnter Voter ID to search: ");
                scanf("%d", &voterID);

                if (voterID <= 0) {
                    printf("\nInvalid Voter ID.\n");
                    break;
                }

                if (searchVoter(voterID) != NULL) {

                    printf("\nVoter ID %d FOUND.\n", voterID);
                    printf("Status: Voter has already voted.\n");
                }
                else {

                    printf("\nVoter ID %d NOT FOUND.\n", voterID);
                    printf("Status: Voter has not voted.\n");
                }

                break;

            /*----------------------------------
               CASE 3: DISPLAY HASH TABLE
            ----------------------------------*/
            case 3:

                if (adminLogin()) {
                    displayHashTable();
                }

                break;

            /*----------------------------------
               CASE 4: DELETE VOTER
            ----------------------------------*/
            case 4:

                if (adminLogin()) {

                    printf("\nEnter Voter ID to delete: ");
                    scanf("%d", &voterID);

                    if (voterID <= 0) {
                        printf("\nInvalid Voter ID.\n");
                    }
                    else {
                        deleteVoter(voterID);
                    }
                }

                break;

            /*----------------------------------
               CASE 5: DISPLAY STATISTICS
            ----------------------------------*/
            case 5:

                displayStatistics();
                break;

            /*----------------------------------
               CASE 6: EXIT
            ----------------------------------*/
            case 6:

                freeAll();

                printf("\n============================================\n");
                printf("Thank you for using the system.\n");
                printf("Exiting program...\n");
                printf("============================================\n");

                return 0;

            /*----------------------------------
               INVALID CHOICE
            ----------------------------------*/
            default:

                printf("\nInvalid choice! Please try again.\n");
        }
    }

    return 0;
}
