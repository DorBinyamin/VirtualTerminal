#ifndef EX1_TERMINAL_H
#define EX1_TERMINAL_H

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include "Folder.h"
#include "FileManager.h"

class Terminal {
    Folder* root;
    static std::string currpath;
    static std::string pathpys; //for if any file in system
    // A map to hold command to function mappings
    std::unordered_map<std::string, std::function<void(const std::vector<std::string>&)>> commandMap;

    // Command handlers
    void handleTouch(const std::vector<std::string>& tokens);
    void handleRemove(const std::vector<std::string>& tokens);
    void handleRead(const std::vector<std::string>& tokens);
    void handleWrite(const std::vector<std::string>& tokens);
    void handleCat(const std::vector<std::string>& tokens);
    void handleWc(const std::vector<std::string>& tokens);
    void handleCopy(const std::vector<std::string>& tokens);
    void handleMove(const std::vector<std::string>& tokens);
    void handleLn(const std::vector<std::string>& tokens);
    void handleMkdir(const std::vector<std::string>& tokens);
    void handleChdir(const std::vector<std::string>& tokens);
    void handleRmdir(const std::vector<std::string>& tokens);
    void handleLs(const std::vector<std::string>& tokens);
    void handleLproot();
    static void handlePwd();
    void handleExit();

public:
    Terminal();
    ~Terminal();
    void executeCommand(const std::string& line);
    static std::vector<std::string> tokenize(const std::string& line);
    static std::string toInternalPath(const std::string& path);
};

#endif //EX1_TERMINAL_H
