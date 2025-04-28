#ifndef EX1_FOLDER_H
#define EX1_FOLDER_H

#include <iostream>
#include <vector>
#include <string>
#include "FileManager.h"

// Folder class represents a directory structure in the file system.
class Folder {
private:
    std::string foldername;  // The name of the folder
    Folder* parent;  // Pointer to the parent folder (nullptr if this is the root)
    std::vector<Folder> subfolders;  // List of subfolders contained within this folder
    std::vector<FileManager> files;  // List of files contained within this folder
public:
    // Constructor initializes a folder with a given name
    explicit Folder(const char* name);

    // Method to create a new folder within the current folder
    void mkdir(const char* foldername);

    // Method to change the current folder to the specified folder
    void chdir(const char* foldername);

    // Method to remove a folder by its name
    void rmdir(const char* foldername);

    // Method to list all subfolders and files in the current folder
    void ls(const char* foldername) const;

    // Method to display the structure of the entire file system from the root
    void lproot() const;

    // Static method to display the current working directory (PWD)
    static void pwd();

    // Method to add a file to the current folder
    void addFile(const FileManager& fm);

    // Method to remove a file by its name
    void removeFile(const std::string& filename);

    // Method to retrieve a file by its name
    FileManager* getFile(const std::string& name);

    // Method to check if a folder exists at the specified path
    bool folderExists(const std::string& fullPath) const;

    // Destructor to clean up the folder and its contents
    ~Folder();
};

#endif //EX1_FOLDER_H
