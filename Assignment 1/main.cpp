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
        
        // Trim whitespace from command
        size_t start = command.find_first_not_of(" \t\r\n");
        if (start == std::string::npos) {
            // Empty or whitespace-only command
            continue;
        }
        size_t end = command.find_last_not_of(" \t\r\n");
        command = command.substr(start, end - start + 1);
        
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
    pid_t pid = fork();
    
    if (pid == 0) {
        // Child process - execute the command using execl with shell
        execl("/bin/sh", "sh", "-c", command.c_str(), (char*)NULL);
        
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
