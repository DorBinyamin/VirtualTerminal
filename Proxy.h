#ifndef EX1_PROXY_H
#define EX1_PROXY_H
#include <fstream>

// Forward declaration of FileManager class
class FileManager;

// Proxy class acts as a middleman to access and modify file data at specific indices
class Proxy {
private:
    FileManager * f;  // Pointer to the FileManager object that the Proxy will work with
    int index;  // The index in the file where the data will be accessed or modified

public:
    // Constructor to initialize the Proxy with a FileManager pointer and an index
    Proxy(const FileManager* file,int idx);

    // Conversion operator to return a character from the file at the given index
    operator char() const;

    // Assignment operator to set a character in the file at the specified index
    Proxy& operator=(char c);
};

#endif //EX1_PROXY_H
