#include <stdio.h>

void split(char*, char*, char*, char);
int findFirstInstanceOf(char*, char);
int strLen(char*);

int main() { 
    char userInput[] = "This is a mutlispace test\0";
    char command[64];
    char arguments[64];

    // while (1) { 
        // printf("~BlackDOS > ");
        // scanf("%s", userInput);
        split(userInput, command, arguments, ' ');
        printf("Command: ");
        printf("%s", command);
        printf(" with arguments: ");
        printf("%s", arguments);
        printf("\n");
    // }


    int result1 = findFirstInstanceOf(command, '\0');
    int result2 = findFirstInstanceOf(arguments, '\0');

    printf("%d, %d\nBasically just showing that we have numbers popping up here", result1, result2);

    return 0;
}

void memCopy(char* from, char* to, int bytes) {
  int i = 0;
  while(i < bytes) {
    to[i] = from[i];
    ++i;
  }
  return;
}

void split(char* original, char* arg1, char* arg2, char delim) { 
    int i = findFirstInstanceOf(original, delim);
    if (i == -1) {  // Character not found
        printf("Character not found\n");
        printf("%s\n", original);
        return;
    } else { // Character found
        int j = 0;
        while (j < i) {
            // copy the first part into arg1
            arg1[j] = original[j];
            printf("%s\n", arg1);
            ++j;
        }
        printf("%d\n", j);
        arg1[j] = '\0'; // Add null terminator onto the first argument
        ++j; // This sets the index of the first part of the arguemnt.
        // Convieniently, this ALSO skips the delimiter character. In this case, we want to discard 
        // the delimiter character.

        // Lord we can hope that the original string is null terminated.
        // If I did my job correctly in the past, theh string should be null terminated.
        int k = 0; // current index in the array arg2
        while (original[j] != '\0') { 
            arg2[k] = original[j];
            ++k;
            ++j;
        }
    }
}

int findFirstInstanceOf(char* string, char delim) { 
    int i = 0;

    while (string[i] != '\0') { 
        if (string[i] == delim) { 
            printf("character found\n");
            printf("%d\n", i);
            return i;
        }
        ++i;
    }
    return -1; // index not found
}

int strLen(char* string) { 
    int len = 0;
    int i = 0;
    
    while (string[i] != '\0') { 
        len++;
        i++;
    }
    len++;
    return len;
}