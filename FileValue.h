#ifndef EX1_FILE_VALUE_H
#define EX1_FILE_VALUE_H

#include "RCObject.h"
#include "FileException.h"
#include "Proxy.h"
#include <fstream>
#include <string>

// FileValue class: Manages a file stream with reference counting via RCObject
class FileValue : public RCObject {
public:
    // Constructor: Opens a file for reading and writing
    explicit FileValue(const char* filename);

    // Copy constructor: Initializes a new FileValue from another
    FileValue(const FileValue& rhs);

    // Assignment operator: Handles copying and cleanup
    FileValue& operator=(const FileValue& rhs);

    // Destructor: Cleans up the file stream
    ~FileValue() override;

    // Pointer to the file stream
    std::fstream* stream;

    // Pointer to the filename
    const char* filename{};

private:
    // Helper function to initialize the file stream
    void init(const char* filename);
};

#endif //EX1_FILE_VALUE_H
