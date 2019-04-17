#include <iostream>
#include <string>
#include <cstring> 
#include <sys/wait.h>
#include <cstdlib> 
#include <unistd.h>
#include <vector>

#define MAX_ARGS 64
#define MAX_ARG_LEN 16
#define MAX_LINE_LEN 80
#define WHITESPACE " ,\t\n"

class command_t { 
public: 
    std::string name; 
    int argc;
    char* argv[MAX_ARGS];
    std::vector<std::string> argumentList;

    void print() { 
        std::cout << "argv\n";
        for (int i = 0; i < argc; ++i) { 
            std::cout << this->argv[i] << std::endl;
        }

        std::cout << "argumentList\n";
        for (int i = 0; i < argumentList.size(); ++i) { 
            std::cout << this->argumentList[i] << std::endl;
        }

        std::cout << "name: " << this->name << std::endl;
    }

    ~command_t() { 
        for (int i = 0; i < argc; ++i) { 
            delete argv[i];
        }
    }

    void clear() { 
        this->name = "";
        this->argc = 0;
        for (int i = 0; i < argc; ++i) { 
            delete argv[i];
        }
        argumentList.clear();
    }
};

int parseCommand(char* , command_t*);
void printPrompt();
void readCommand(char*);
void parseShortcut(command_t*);
void manual();

int main() { 
    int pid;
    int status;
    char cmdLine[MAX_LINE_LEN];
    command_t command; 

    while (1) { 
        printPrompt();
        readCommand(cmdLine);
        parseCommand(cmdLine, &command);

        command.print();
        if ((pid = fork()) == 0) { 
            std::cout << "Hello from child process\n";
            execvp(command.name.c_str(), command.argv);
        }

        // command.argv[command.argc] = nullptr;
        // manual();

        while(wait(&status) == pid) { 

        }
        command.clear();
    }
    return 0;
}

void printPrompt() { 
    std::cout << "Linux khk8|> ";
}

void readCommand(char* buffer) { 
    fgets(buffer, MAX_LINE_LEN, stdin);
}

int parseCommand(char* cLine, command_t* command) { 
    int argc;

    std::string cLineString(cLine);
    // std::cout << cLineString << std::endl;
    
    std::size_t index = 0;
    char delim = ' ';
    std::string token;

    if (cLineString.find(delim) == std::string::npos) { 
        token = cLineString;
        command->argumentList.push_back(token);
        cLineString.clear();
    }

    while ((index = cLineString.find(delim)) != std::string::npos) { 
        token = cLineString.substr(0, index);
        command->argumentList.push_back(token);
        cLineString.erase(0, index + 1);

        if (cLineString.find(delim) == std::string::npos) { 
            token = cLineString;
            command->argumentList.push_back(token);
            cLineString.clear();
        }
    }

    std::cout << "Leftover arugments: ";
    std::cout << cLineString << std::endl;

    std::cout << "Command line argument list: " << std::endl;
    for (int i = 0; i < command->argumentList.size(); ++i) { 
        std::cout << command->argumentList[i] << std::endl;
    }
    
    command->argc = command->argumentList.size();
    command->name = command->argumentList[0];
    int i = 0;
    for ( ; i < command->argumentList.size(); ++i) {
        command->argv[i] = new char[MAX_ARG_LEN]; 
        strcpy(command->argv[i], command->argumentList[i].c_str());
    }

    command->argv[i] = nullptr;

    // TODO parse shortcuts

    return 1;
}

// Wrote this in the starter file, in C. too lazy to redo it
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