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
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    
    // Parse command and arguments
    size_t spacePos = command.find(' ');
    std::string fullCommand;
    
    if (spacePos == std::string::npos) {
        // No arguments - try to find executable
        std::string prog = command;
        
        // Try current directory first with .exe extension
        fullCommand = prog + ".exe";
        if (GetFileAttributes(fullCommand.c_str()) == INVALID_FILE_ATTRIBUTES) {
            // Try without .exe extension
            fullCommand = prog;
            if (GetFileAttributes(fullCommand.c_str()) == INVALID_FILE_ATTRIBUTES) {
                // Try system commands
                fullCommand = "cmd /c " + command;
            }
        }
    } else {
        // Has arguments
        std::string prog = command.substr(0, spacePos);
        std::string args = command.substr(spacePos + 1);
        
        // Try current directory first with .exe extension
        fullCommand = prog + ".exe " + args;
        if (GetFileAttributes((prog + ".exe").c_str()) == INVALID_FILE_ATTRIBUTES) {
            fullCommand = prog + " " + args;
            if (GetFileAttributes(prog.c_str()) == INVALID_FILE_ATTRIBUTES) {
                // Try system commands
                fullCommand = "cmd /c " + command;
            }
        }
    }
    
    // Convert string to char array for CreateProcess
    char* cmdLine = new char[fullCommand.length() + 1];
    strcpy(cmdLine, fullCommand.c_str());
    
    // Start the child process
    if (!CreateProcess(NULL,   // No module name (use command line)
        cmdLine,               // Command line
        NULL,                  // Process handle not inheritable
        NULL,                  // Thread handle not inheritable
        FALSE,                 // Set handle inheritance to FALSE
        0,                     // No creation flags
        NULL,                  // Use parent's environment block
        NULL,                  // Use parent's starting directory
        &si,                   // Pointer to STARTUPINFO structure
        &pi)                   // Pointer to PROCESS_INFORMATION structure
    ) {
        std::cerr << "Command not found: " << command << std::endl;
        delete[] cmdLine;
        return;
    }
    
    // Wait until child process exits
    WaitForSingleObject(pi.hProcess, INFINITE);
    
    // Close process and thread handles
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    delete[] cmdLine;
}
