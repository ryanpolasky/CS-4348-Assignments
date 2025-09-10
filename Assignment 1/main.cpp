//
// Created by ryanp on 9/9/2025.
//

#include "main.h"

int main() {
    runShell();
    return 0;
}

void runShell() {
    std::string command;
    
    while (true) {
        std::cout << "m% ";
        std::getline(std::cin, command);
        
        // Check for exit command
        if (command == "myexit") {
            break;
        }
        
        // Skip empty commands
        if (command.empty()) {
            continue;
        }
        
        executeCommand(command);
    }
}

void executeCommand(const std::string& command) {
    // Parse command and arguments
    size_t spacePos = command.find(' ');
    std::string prog;
    std::string args;
    
    if (spacePos == std::string::npos) {
        prog = command;
        args = "";
    } else {
        prog = command.substr(0, spacePos);
        args = command.substr(spacePos + 1);
    }
    
    // Check if executable exists in current directory
    struct stat buffer;
    bool foundLocal = (stat(prog.c_str(), &buffer) == 0);
    
    pid_t pid = fork();
    
    if (pid == 0) {
        // Child process
        if (foundLocal) {
            // Execute local program
            if (args.empty()) {
                execlp(prog.c_str(), prog.c_str(), (char*)NULL);
            } else {
                // Simple argument parsing - split on spaces
                std::string fullCmd = prog + " " + args;
                execl("/bin/sh", "sh", "-c", fullCmd.c_str(), (char*)NULL);
            }
        } else {
            // Try to execute as system command
            if (args.empty()) {
                execlp(prog.c_str(), prog.c_str(), (char*)NULL);
            } else {
                std::string fullCmd = prog + " " + args;
                execl("/bin/sh", "sh", "-c", fullCmd.c_str(), (char*)NULL);
            }
        }
        
        // If we get here, exec failed
        std::cerr << "Command not found: " << command << std::endl;
        exit(1);
    } else if (pid > 0) {
        // Parent process - wait for child to complete
        int status;
        waitpid(pid, &status, 0);
    } else {
        // Fork failed
        std::cerr << "Failed to create process for: " << command << std::endl;
    }
}
