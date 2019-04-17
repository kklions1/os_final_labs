// ACADEMIC INTEGRITY PLEDGE
//
// - I have not used source code obtained from another student nor
//   any other unauthorized source, either modified or unmodified.
//
// - All source code and documentation used in my program is either
//   my original work or was derived by me from the source code
//   published in the textbook for this course or presented in
//   class.
//
// - I have not discussed coding details about this project with
//   anyone other than my instructor. I understand that I may discuss
//   the concepts of this program with other students and that another
//   student may help me debug my program so long as neither of us
//   writes anything during the discussion or modifies any computer
//   file during the discussion.
//
// - I have violated neither the spirit nor letter of these restrictions.
//
//
//
// Signed:________________Kevin Klions____________ Date:__4-8-19_______

// 3460:426 Lab 4A - Basic C shell

/* Basic shell */

/*
 * This is a very minimal shell. It finds an executable in the
 * PATH, then loads it and executes it (using execv). Since
 * it uses "." (dot) as a separator, it cannot handle file
 * names like "minishell.h"
 *
 * The focus on this exercise is to use fork, PATH variables,
 * and execv. 
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#define MAX_ARGS	64
#define MAX_ARG_LEN	16
#define MAX_LINE_LEN	80
#define WHITESPACE	" ,\t\n"

struct command_t {
   char *name;
   int argc;
   char *argv[MAX_ARGS];
};

/* Function prototypes */
int parseCommand(char *, struct command_t *);
void printPrompt();
void readCommand(char *);
void parseShortcut(struct command_t*);
void manual();
// void wait(int*); 

int main(int argc, char *argv[]) {
   int pid;
   int status;
   char cmdLine[MAX_LINE_LEN];
   struct command_t command;

   while (1) {
      printPrompt();
      /* Read the command line and parse it */
      readCommand(cmdLine);

      parseCommand(cmdLine, &command);
    //   ...
      command.argv[command.argc] = NULL;

      /* Create a child process to execute the command */
      if ((pid = fork()) == 0) {
         /* Child executing command */
         execvp(command.name, command.argv);
      }
      /* Wait for the child to terminate */
      wait(&status);
   }

   /* Shell termination */
   printf("\n\n shell: Terminating successfully\n");
   return 0;
}

/* End basic shell */

/* Parse Command function */

/* Determine command name and construct the parameter list.
 * This function will build argv[] and set the argc value.
 * argc is the number of "tokens" or words on the command line
 * argv[] is an array of strings (pointers to char *). The last
 * element in argv[] must be NULL. As we scan the command line
 * from the left, the first token goes in argv[0], the second in
 * argv[1], and so on. Each time we add a token to argv[],
 * we increment argc.
 */
int parseCommand(char *cLine, struct command_t *cmd) {
   int argc;
   char **clPtr;
   /* Initialization */
   clPtr = &cLine;	/* cLine is the command line */
   argc = 0;
   cmd->argv[argc] = (char *) malloc(MAX_ARG_LEN);
   /* Fill argv[] */
   while ((cmd->argv[argc] = strsep(clPtr, WHITESPACE)) != NULL) {
      cmd->argv[++argc] = (char *) malloc(sizeof(char) * MAX_ARG_LEN);
   }

   printf("%s\n", cmd->argv[0]);
   parseShortcut(cmd);
   printf("%s\n", cmd->argv[0]);

   /* Set the command name and argc */
   cmd->argc = argc-1;
   cmd->name = (char *) malloc(sizeof(cmd->argv[0]));
   strcpy(cmd->name, cmd->argv[0]);
   return 1;
}

/* End parseCommand function */

/* Print prompt and read command functions - pp. 79-80 */

void printPrompt() {
   /* Build the prompt string to have the machine name,
    * current directory, or other desired information
    */
   char promptString[] = "Linux khk8|> ";
   printf("%s ", promptString);
}

void readCommand(char *buffer) {
   /* This code uses any set of I/O functions, such as those in
    * the stdio library to read the entire command line into
    * the buffer. This implementation is greatly simplified,
    * but it does the job.
    */
   fgets(buffer, 80, stdin);
}

/* End printPrompt and readCommand */

void parseShortcut(struct command_t* cmd) { 
   if (strcmp(cmd->argv[0], "C") == 0) { 
      printf("Copy command was found\n");

      
      printf("argv[1]: %s\n", cmd->argv[1]);
      printf("argv[2]: %s\n", cmd->argv[2]);
      strcpy(cmd->argv[0], "cp");
      printf("command changed\n");
      printf("argv[1]: %s\n", cmd->argv[1]);
      printf("argv[2]: %s\n", cmd->argv[2]);
   } else if (strcmp(cmd->argv[0], "D") == 0) { 
      strcpy(cmd->argv[0], "rm");      
   } else if (strcmp(cmd->argv[0], "E") == 0) { 
      strcpy(cmd->argv[0], "echo");
      printf("argv[0]: %s\n", cmd->argv[0]);
      printf("argv[1]: %s\n", cmd->argv[1]);
      // printf("size argv[0]: %d\n", sizeof(cmd->argv[0]));
   } else if (strcmp(cmd->argv[0], "H") == 0) { 
      // TODO print my user manual
      manual();
   } else if (strcmp(cmd->argv[0], "L") == 0) { 
      strcpy(cmd->argv[0], "ls");
      strcpy(cmd->argv[1], "-a");
      // ls -a
   } else if (strcmp(cmd->argv[0], "M") == 0) { 
      // touch file
   } else if (strcmp(cmd->argv[0], "P") == 0) { 
      // TODO print contents of file to screen
   } else if (strcmp(cmd->argv[0], "Q") == 0) { 
      // TODO quit the shell
   } else if (strcmp(cmd->argv[0], "S") == 0) { 
      // Launch a web browser, probs firefox
   } else if (strcmp(cmd->argv[0], "W") == 0) { 
      strcpy(cmd->argv[0], "clear");
      // clear
   } else if (strcmp(cmd->argv[0], "X") == 0) { 
      // TODO execute file 
   } else { 
      printf("some other command was found\n");
      // printf("argv[1]: %s\n", cmd->argv[1]);
      // printf("argv[2]: %s\n", cmd->argv[2]);   
   }
}

void manual() { 
   printf("manual printed\n");
   printf("C [file1] [file2] - Copy file1 to file2\n\n");
   printf("D [file] - delete the file\n\n");
   printf("E [comment] - echo the given comment\n\n");
   printf("H - Display this help prompt\n\n");
   printf("L - List the contents of the directory\n\n");
   printf("M [filename] - Create a file with the given name\n\n");
   printf("P [file] - Print the contents of the given file\n\n");
   printf("Q - Quit the shell\n\n");
   printf("S - Launch Firefox\n\n");
   printf("W - Clear thet screen\n\n");
   printf("X [program] - Execute the given program\n\n");
}