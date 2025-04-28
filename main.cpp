#include <iostream>
#include <string>
#include "Terminal.h"

int main() {
    Terminal terminal; //Create mini-terminal
    std::string line;
    while (std::getline(std::cin, line)) {
        terminal.executeCommand(line); //Read the command from the user and action until exit
    }

    return 0;
}



