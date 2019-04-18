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

int parseCommand(std::string& , command_t*);
void printPrompt();
void readCommand(std::string&);
void parseShortcut(command_t*);
void manual();

int main() { 
    int pid;
    int status;
    char cmdLine[MAX_LINE_LEN];
    std::string input;
    command_t command; 

    while (true) { 
        printPrompt();
        readCommand(input);
        parseCommand(input, &command);

        if ((pid = fork()) == 0) { 
            execvp(command.name.c_str(), command.argv);
        }

        while(wait(&status) == pid) { 

        }
        command.clear();
    }
    return 0;
}

void printPrompt() { 
    std::cout << "Linux khk8|> ";
}

void readCommand(std::string& buffer) { 
    // fgets(buffer, MAX_LINE_LEN, stdin);
    std::getline(std::cin, buffer);
}

int parseCommand(std::string& cLineString, command_t* command) { 
    // std::string cLineString(cLine);    
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

    parseShortcut(command);

    command->name = command->argumentList[0];
    command->argc = 0;

    for ( ; command->argc < command->argumentList.size(); command->argc++) {
        command->argv[command->argc] = new char[MAX_ARG_LEN]; 
        strcpy(command->argv[command->argc], command->argumentList[command->argc].c_str());
    }

    command->argv[command->argc] = nullptr;

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

void parseShortcut(command_t* command) { 
    if (command->argumentList[0] == std::string("C")) { 
        command->argumentList[0].replace(command->argumentList[0].begin(), command->argumentList[0].end(), std::string("cp"));
    
    } else if (command->argumentList[0] == "E") {    
        command->argumentList[0].replace(command->argumentList[0].begin(), command->argumentList[0].end(), std::string("echo"));

    } else if (command->argumentList[0] == "H") { 
        manual();

    } else if (command->argumentList[0] == "L") { 
        command->argumentList[0].replace(command->argumentList[0].begin(), command->argumentList[0].end(), std::string("ls"));
        command->argumentList.push_back(std::string("-la"));

    } else if (command->argumentList[0] == "M") { 
        command->argumentList[0].replace(command->argumentList[0].begin(), command->argumentList[0].end(), std::string("nano"));

    } else if (command->argumentList[0] == "P") { 
        command->argumentList[0].replace(command->argumentList[0].begin(), command->argumentList[0].end(), std::string("more"));

    } else if (command->argumentList[0] == "Q") { 
        std::cout << "Goodbye!\n";
        exit(0);

    } else if (command->argumentList[0] == "S") { 
        command->argumentList[0].replace(command->argumentList[0].begin(), command->argumentList[0].end(), std::string("firefox"));

    } else if (command->argumentList[0] == "W") { 
        command->argumentList[0].replace(command->argumentList[0].begin(), command->argumentList[0].end(), std::string("clear"));

    } else if (command->argumentList[0] == "X") { 
        std::string file = command->argumentList[1];
        std::string exec = "./" + file;
        command->argumentList.clear();
        command->argumentList.push_back(exec);

    } else if (command->argumentList[0] == "D") { 
        command->argumentList[0].replace(command->argumentList[0].begin(), command->argumentList[0].end(), std::string("rm"));

    }
}