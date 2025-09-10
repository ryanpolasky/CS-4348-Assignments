//
// Created by ryanp on 9/9/2025.
//

#ifndef CS_4348_ASSIGNMENTS_MAIN_H
#define CS_4348_ASSIGNMENTS_MAIN_H

#include <iostream>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>

void runShell();
void executeCommand(const std::string& command);

#endif //CS_4348_ASSIGNMENTS_MAIN_H
