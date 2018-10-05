#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

struct instruction {
    int state;
    char read;
    char write;
    char move;
    int newState;
};

struct node {
    char num;
    struct node *right;
    struct node *left;
};

typedef struct instruction INST;
typedef struct node NODE;

void addLeftNode(NODE **, char);
void addRightNode(NODE **, char);
void printTape(NODE **);

int main( ) {
    NODE *head = NULL, *node = NULL;
    FILE *inputFile;
    char input[20];
    char fileName[30];
    int numStates, startState, endState;
    char read, write, move;
    int state, newState;
    char trash;
    int j, sameState = -1;

    //Get File from User
    printf("Name of File: ");
    scanf("%s", fileName);
    printf("Input File: %s\n", fileName);
    inputFile = fopen("/home/andrew/Dropbox/proglang/project1/tape.txt", "r");
    fscanf(inputFile, "%s", input);

    //Create the list
    for(int i = 0; i < strlen(input); i++) {
        addRightNode(&head, input[i]);
    }

    printf("Initial Tape Contents: ");
    printTape(&head);
    printf("\n");

    printf("Writing tape...\n");

    //Create the 2D array
    fscanf(inputFile, "%d", &numStates);
    fscanf(inputFile, "%d", &startState);
    fscanf(inputFile, "%d", &endState);
    fscanf(inputFile, "%c", &trash);

    //Malloc's the Array
    INST **instArray = (INST **) malloc(numStates * sizeof(INST));
    for(int i = 0; i < numStates; i++) {
        instArray[i] = (INST *) malloc(256 * sizeof(INST));
    }

    //Fills Array with Instructions
    while(feof(inputFile) == 0) {
        fscanf(inputFile, "%d %c %c %c %d", &state, &read, &write, &move, &newState);
        if(state == sameState) {
            j++;
        } else {
            j = 0;
            sameState = state;
        }
        instArray[state][j].state = state;
        instArray[state][j].read = read;
        instArray[state][j].write = write;
        instArray[state][j].move = move;
        instArray[state][j].newState = newState;
        fscanf(inputFile, "%c", &trash);
    }

    //Run Da Machine
    NODE* current = head;
    int s = 0, t = 0;
    char r, w, m;
    char compare;
    char dirCompare = 'L';
    while(s != endState) {

        //Compares the number from the instruction to the one on the tape.
        r = instArray[s][t].read;
        compare = current->num;

        //If they compare
        if(compare == r) {
            //Write the write number onto tape
            current->num = instArray[s][t].write;
            //Move instruction L or R
            dirCompare = instArray[s][t].move;
            if(dirCompare == 'L') {
                if(current->left == NULL) {
                    addLeftNode(&head, 'B');
                }
                current = current->left;
            } else {
                if(current->right == NULL) {
                    addRightNode(&head, 'B');
                }
                current = current->right;
            }
            // Changes the state
            if(s != instArray[s][t].newState) {
                s = instArray[s][t].newState;
                t = 0;
            } else {
                s = instArray[s][t].newState;
            }
        } else { //If the num on the tape and the instruction don't compare
            t = -1;
            do{
                t++;
                r = instArray[s][t].read;
            } while(r != current->num);
        }
    }
    printf("Final Tape Contents: ");
    printTape(&head);

    return 0;
}


void addLeftNode(NODE **head, char num) {
    NODE* node = malloc(sizeof(NODE));

    node->num = num;
    node->right = *head;
    node->left = NULL;

    if((*head) != NULL) {
        (*head)->left = node;
    }

    (*head) = node;
}

void addRightNode(NODE **head, char num) {
    NODE* node = malloc(sizeof(NODE));
    NODE *prev = *head;

    node->num = num;
    node->right = NULL;

    //Use this to build the list initially.
    if(*head == NULL) {
        node->left = NULL;
        *head = node;
    } else {
        //Only way to traverse to end.
        while(prev->right != NULL) {
            prev = prev->right;
        }
        prev->right = node;
        node->left = prev;
    }
}

void printTape(NODE **head) {
    NODE* node = *head;
    while(node->right != NULL) {
        printf("%c ", node->num);
        node = node->right;
    }
    printf("%c", node->num);
}