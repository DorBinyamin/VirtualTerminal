#ifndef EX1_FILE_MANAGER_H
#define EX1_FILE_MANAGER_H

#include "RCPtr.h"
#include "FileValue.h"
#include "Proxy.h"

// The FileManager class manages file operations using reference-counted pointers
class FileManager {
    friend class Proxy; // Allow Proxy class to access private members
private:
    void validateReadStream() const; // Validate if the file stream is ready for reading
    void validateWriteStream();      // Validate if the file stream is ready for writing
    void validateIndex(int i) const; // Validate index bounds when accessing the file content
    RCPtr<FileValue> file;            // Smart pointer to handle the file data
    std::string namefile;             // Name of the file
    int count{};                      // Number of characters written to the file
public:
    FileManager() : file(nullptr) {} // Default constructor initializing file to nullptr
    explicit FileManager(const char* filename); // Constructor that opens/creates a file
    FileManager& operator=(const FileManager& other); // Copy assignment operator
    Proxy operator[](int i) const; // Read-only access to a character via Proxy
    Proxy operator[](int i);       // Write access to a character via Proxy
    void touch(const char* filename); // Create a new empty file
    void copy(FileManager& target);   // Copy contents to another FileManager target
    void remove(const char* filename); // Delete the specified file
    void cat(const char* filename) const; // Print file contents to console
    void wc(const char* filename) const;  // Print word count, line count, and char count
    void ln(FileManager& target); // Create a symbolic link (share the file pointer)
    std::string getFileName() const; // Get the file name
    int getRefCount() const { return file->getRefCount(); } // Get current reference count
    ~FileManager() = default; // Default destructor
};

#endif // EX1_FILE_MANAGER_H
