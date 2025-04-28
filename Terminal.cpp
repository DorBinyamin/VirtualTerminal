#include "Terminal.h"
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cstdlib>

std::string Terminal::pathpys = "V#";
std::string Terminal::currpath;
Terminal::Terminal() {
    // Initialize the root folder with the name "V"
    root = new Folder("V");

    // Initialize the command map with function mappings
    // Each command is mapped to a corresponding handler method
    commandMap["touch"] = [this](const std::vector<std::string>& tokens) { handleTouch(tokens); };
    commandMap["remove"] = [this](const std::vector<std::string>& tokens) { handleRemove(tokens); };
    commandMap["read"] = [this](const std::vector<std::string>& tokens) { handleRead(tokens); };
    commandMap["write"] = [this](const std::vector<std::string>& tokens) { handleWrite(tokens); };
    commandMap["cat"] = [this](const std::vector<std::string>& tokens) { handleCat(tokens); };
    commandMap["wc"] = [this](const std::vector<std::string>& tokens) { handleWc(tokens); };
    commandMap["copy"] = [this](const std::vector<std::string>& tokens) { handleCopy(tokens); };
    commandMap["move"] = [this](const std::vector<std::string>& tokens) { handleMove(tokens); };
    commandMap["ln"] = [this](const std::vector<std::string>& tokens) { handleLn(tokens); };
    commandMap["mkdir"] = [this](const std::vector<std::string>& tokens) { handleMkdir(tokens); };
    commandMap["chdir"] = [this](const std::vector<std::string>& tokens) { handleChdir(tokens); };
    commandMap["rmdir"] = [this](const std::vector<std::string>& tokens) { handleRmdir(tokens); };
    commandMap["ls"] = [this](const std::vector<std::string>& tokens) { handleLs(tokens); };
    commandMap["lproot"] = [this](const std::vector<std::string>& tokens) { handleLproot(); };
    commandMap["pwd"] = [](const std::vector<std::string>& tokens) { handlePwd(); };
    commandMap["exit"] = [this](const std::vector<std::string>& tokens) { handleExit(); };
}

Terminal::~Terminal() {
    // Clean up the root folder when the terminal object is destroyed
    delete root;
}

// Tokenize the input line into individual command tokens
std::vector<std::string> Terminal::tokenize(const std::string& line) {
    std::istringstream stream(line);
    std::string token;
    std::vector<std::string> tokens;
    while (stream >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

// Convert a user path to an internal representation by replacing '/' with '#'
std::string Terminal::toInternalPath(const std::string& path) {
    std::string internal = path;
    std::replace(internal.begin(), internal.end(), '/', '#');
    return internal;
}

// Execute the command by first tokenizing the input line and then calling the corresponding handler
void Terminal::executeCommand(const std::string& line) {
    auto tokens = tokenize(line);
    if (tokens.empty()) return;

    const std::string& cmd = tokens[0];

    try {
        // Look for the command in the map and call the corresponding handler if found
        if (commandMap.find(cmd) != commandMap.end()) {
            commandMap[cmd](tokens);
        } else {
            std::cerr << "Unknown command or wrong number of arguments." << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
    }
}

// Handler for the 'touch' command: Creates a file in the root folder
void Terminal::handleTouch(const std::vector<std::string>& tokens) {
    if (tokens.size() == 2) {
        const std::string& userPath = tokens[1];
        std::string internal = toInternalPath(userPath);
        FileManager fm(internal.c_str());
        fm.touch(internal.c_str());
        root->addFile(fm);
    }
}

// Handler for the 'remove' command: Removes a file from the root folder
void Terminal::handleRemove(const std::vector<std::string>& tokens) {
    if (tokens.size() == 2) {
        const std::string& userPath = tokens[1];
        std::string internal = toInternalPath(userPath);
        root->removeFile(internal);
    }
}

// Handler for the 'read' command: Reads a file at the given path and outputs its content at the specified index
void Terminal::handleRead(const std::vector<std::string>& tokens) {
    if (tokens.size() == 3) {
        const std::string& userPath = tokens[1];
        int index = std::stoi(tokens[2]);
        std::string internal = toInternalPath(userPath);
        FileManager* file = root->getFile(internal);
        if (!file) {
            std::cerr << "ERROR: File not found in root folder." << std::endl;
        } else {
            std::cout << (*file)[index] << std::endl;
        }
    }
}

// Handler for the 'write' command: Writes a value to the file at the specified index
void Terminal::handleWrite(const std::vector<std::string>& tokens) {
    if (tokens.size() == 4) {
        const std::string& userPath = tokens[1];
        int index = std::stoi(tokens[2]);
        const std::string& strValue = tokens[3];

        if (strValue.size() != 1) {
            std::cerr << "ERROR: Value must be exactly one character." << std::endl;
        } else {
            char value = strValue[0];
            std::string internal = toInternalPath(userPath);
            FileManager* file = root->getFile(internal);
            if (!file) {
                std::cerr << "ERROR: File not found in root folder." << std::endl;
            } else {
                (*file)[index] = value;
            }
        }
    }
}

// Handler for the 'cat' command: Displays the content of a file
void Terminal::handleCat(const std::vector<std::string>& tokens) {
    if (tokens.size() == 2) {
        const std::string& userPath = tokens[1];
        std::string internal = toInternalPath(userPath);
        FileManager* file = root->getFile(internal);

        if (!file) {
            std::cerr << "ERROR: File not found in root folder." << std::endl;
        } else {
            file->cat(file->getFileName().c_str());
        }
    }
}

// Handler for the 'wc' command: Displays word count of a file
void Terminal::handleWc(const std::vector<std::string>& tokens) {
    if (tokens.size() == 2) {
        const std::string& userPath = tokens[1];
        std::string internal = toInternalPath(userPath);
        FileManager* file = root->getFile(internal);

        if (!file) {
            std::cerr << "ERROR: File not found in root folder." << std::endl;
        } else {
            file->wc(file->getFileName().c_str());
        }
    }
}

// Handler for the 'copy' command: Copies a file to a new destination
void Terminal::handleCopy(const std::vector<std::string>& tokens) {
    if (tokens.size() == 3) {
        const std::string& userSrc = tokens[1];
        const std::string& userDst = tokens[2];

        std::string dstInternal = toInternalPath(userDst);
        if (userDst[0] != 'V') { //this is for if the target pysc its mean nor begin with V/
            dstInternal = "V#" + userDst;
        }

        // Check if the destination folder exists
        if (!root->folderExists(dstInternal)) {
            std::cerr << "ERROR: Destination folder not found in root folder." << std::endl;
            return;
        }

        if (userSrc[0] != 'V') { // Source is a physical file
            std::string resolvedSrc = Terminal::pathpys + userSrc;
            FileManager temp(resolvedSrc.c_str());
            temp.touch(resolvedSrc.c_str());
            FileManager dest(dstInternal.c_str());
            dest.touch(dstInternal.c_str());
            temp.copy(dest);
            root->addFile(temp);
            root->addFile(dest);
        } else { // Source is virtual file
            std::string srcInternal = toInternalPath(userSrc);
            FileManager* srcFile = root->getFile(srcInternal);
            if (!srcFile) {
                std::cerr << "ERROR: Source file not found in root folder." << std::endl;
            } else {
                FileManager* dstFile = root->getFile(dstInternal);
                if (!dstFile) {
                    FileManager fm(dstInternal.c_str());
                    fm.touch(dstInternal.c_str());
                    srcFile->copy(fm);
                    root->addFile(fm);
                } else {
                    srcFile->copy(*dstFile);
                }
            }
        }
    }
}


// Handler for the 'move' command: Moves a file to a new folder
void Terminal::handleMove(const std::vector<std::string>& tokens) {
    if (tokens.size() == 3) {
        const std::string& userSrc = tokens[1];
        std::string srcInternal = toInternalPath(userSrc);
        FileManager* srcFile = root->getFile(srcInternal);
        if (!srcFile) {
            std::cerr << "ERROR: Source file not found in root folder." << std::endl;
        } else {
            const std::string& userDst = tokens[2];
            std::string dstInternal = toInternalPath(userDst);
            if (!root->folderExists(dstInternal)) {
                std::cerr << "ERROR: Destination folder not found in root folder." << std::endl;
            } else {
                FileManager* dstFile = root->getFile(dstInternal);
                if (!dstFile) {
                    FileManager fm(dstInternal.c_str());
                    fm.touch(dstInternal.c_str());
                    srcFile->copy(fm);
                    root->addFile(fm);
                } else {
                    srcFile->copy(*dstFile);
                }
                root->removeFile(srcInternal);
            }
        }
    }
}

// Handler for the 'ln' command: Creates a symbolic link to a file
void Terminal::handleLn(const std::vector<std::string>& tokens) {
    if (tokens.size() == 3) {
        const std::string& userSrc = tokens[1];
        const std::string& userDst = tokens[2];
        std::string srcInternal = toInternalPath(userSrc);
        std::string dstInternal = toInternalPath(userDst);
        bool srcFolderExists = root->folderExists(srcInternal);
        bool dstFolderExists = root->folderExists(dstInternal);
        FileManager* srcFile = root->getFile(srcInternal);
        FileManager* dstFile = root->getFile(dstInternal);
        if (!srcFolderExists || !dstFolderExists || !srcFile || !dstFile) {
            std::cerr << "ERROR: Source/Destination folder or file not found in root folder." << std::endl;
        } else {
            srcFile->ln(*dstFile);
        }
    }
}

// Handler for the 'mkdir' command: Creates a new directory
void Terminal::handleMkdir(const std::vector<std::string>& tokens) {
    if (tokens.size() == 2) {
        const std::string& path = tokens[1];
        if (path.empty() || path.back() != '/') {
            std::cerr << "Error: Path must end with '/'" << std::endl;
            return;
        }
        root->mkdir(path.c_str());
    }
}


// Handler for the 'chdir' command: Changes the current directory
void Terminal::handleChdir(const std::vector<std::string>& tokens) {
    if (tokens.size() == 2) {
        const std::string& path = tokens[1];
        if (path.empty() || path.back() != '/') {
            std::cout << "Error: Path must end with '/'" << std::endl;
            return;
        }
        root->chdir(path.c_str());
        Terminal::currpath=path;
        Terminal::pathpys = toInternalPath(path);
    }
}


// Handler for the 'rmdir' command: Removes a directory
void Terminal::handleRmdir(const std::vector<std::string>& tokens) {
    if (tokens.size() == 2) {
        root->rmdir(tokens[1].c_str());
    }
}

// Handler for the 'ls' command: Lists files and directories
void Terminal::handleLs(const std::vector<std::string>& tokens) {
    if (tokens.size() == 2) {
        if(Terminal::currpath.empty())
            root->ls("V");
        else if(Terminal::currpath == tokens[1])
            root->ls(tokens[1].c_str());
        else
            std::cerr <<"Invalid input"<<std::endl;
    }
}

// Handler for the 'lproot' command: Lists all files in the root directory
void Terminal::handleLproot() {
    root->lproot();
}

// Handler for the 'pwd' command: Prints the current working directory
void Terminal::handlePwd() {
    Folder::pwd();
}

// Handler for the 'exit' command: Exits the terminal and cleans up
void Terminal::handleExit() {
    delete root;
    exit(0);
}
