#include <iostream>
#include "FileValue.h"

// Constructor: Opens the file for reading and writing, moves to the end of file
FileValue::FileValue(const char* filename) : filename(filename) {
    stream = new std::fstream(filename, std::ios::in | std::ios::out | std::ios::ate);
}

// Initialize the file stream for binary read/write mode
void FileValue::init(const char* filname) {
    stream = new std::fstream(filename, std::ios::in | std::ios::out | std::ios::binary);
    this->filename = filname;
}

// Copy constructor: uses RCObject's copy and initializes the new stream
FileValue::FileValue(const FileValue& rhs) : RCObject(rhs), stream(nullptr) {
    init(rhs.filename);
}

// Assignment operator: properly handles resource management when copying
FileValue& FileValue::operator=(const FileValue& rhs) {
    if (this != &rhs) {
        if (stream) {
            stream->close();
            delete stream;
        }
        init(rhs.filename);
    }
    return *this;
}

// Destructor: closes and deletes the file stream
FileValue::~FileValue() {
    if (stream) {
        stream->close();
        delete stream;
        stream = nullptr;
    }
}
