#include "FileManager.h"
#include <iostream>
#include <sstream>


// Constructor
FileManager::FileManager(const char* filename)
        : file(new FileValue(filename)), namefile(filename), count(0) {}

// Const operator[]
Proxy FileManager::operator[](int i) const {
    validateReadStream();
    validateIndex(i);
    return { this, i };
}

// Non-const operator[]
Proxy FileManager::operator[](int i) {
    validateWriteStream();
    validateIndex(i);
    return { this, i };
}

// Create file
void FileManager::touch(const char* filename) {
    this->namefile = filename;
    file->stream->open(namefile, std::ios::out | std::ios::in);
    if (!file->stream->is_open()) {
        file->stream->open(namefile, std::ios::out | std::ios::trunc);
    }
    file->stream->flush();
    file->stream->close();
}

// Copy contents to another FileManager target
void FileManager::copy(FileManager& target) {
    file->stream->open(this->namefile, std::ios::in | std::ios::binary);
    if (!file->stream->is_open()) {
        throw FileException(FileException::ErrorType::CopyError,
                            "Failed to open source file: " + std::string(this->namefile));
    }

    target.file->stream->open(target.namefile, std::ios::out | std::ios::binary | std::ios::trunc);
    if (!file->stream->is_open()) {
        target.file->stream->open(target.namefile, std::ios::out | std::ios::binary | std::ios::trunc);
    }
    *target.file->stream << file->stream->rdbuf();
    target.count = this->count;
    target.file->stream->flush();
    target.file->stream->close();
    file->stream->flush();
    file->stream->close();
}



// Remove file content
void FileManager::remove(const char* filename) {
    if (file->stream->is_open()) {
        file->stream->close();
    }
    if(!file->stream->good()) return; //this is for if the file  not exist
    if (std::remove(filename) != 0) {
        throw FileException(FileException::ErrorType::DeleteError,
                            "Failed to delete file: " + std::string(filename));
    }
    file = RCPtr<FileValue>(nullptr);
}



// Print file content
void FileManager::cat(const char* filename) const {
    file->stream->open(filename, std::ios::in);
    if (!file->stream->is_open()) {
        throw FileException(FileException::ErrorType::NotOpen,
                            "File stream is not open.");
    }
    std::string line;
    while (std::getline(*file->stream, line)) {
        std::cout << line << std::endl;
    }
    file->stream->close();
}

// Print word count, line count, and char count
void FileManager::wc(const char* filename) const {
    file->stream->open(filename, std::ios::in);
    if (!file->stream->is_open()) {
        throw FileException(FileException::ErrorType::NotOpen,
                            "File stream is not open.");
    }
    std::string line;
    int lineCount = 0, wordCount = 0, charCount = 0;
    while (std::getline(*file->stream, line)) {
        ++lineCount;
        charCount += static_cast<int>(line.size());
        std::istringstream iss(line);
        std::string word;
        while (iss >> word) {
            ++wordCount;
        }
    }
    std::cout << "Lines: " << lineCount
              << ", Words: " << wordCount
              << ", Characters: " << charCount << std::endl;
    file->stream->close();
}

// Create symbolic link (shared pointer)- symmetry from the email of Ofer shir
void FileManager::ln(FileManager& target) {
    if (!file.operator->()) {
        throw FileException(FileException::ErrorType::FileNotFound,
                            "Source file is invalid or uninitialized");
    }
    if (!file->isShareable()) {
        throw FileException(FileException::ErrorType::CopyError,
                            "Source file is marked as unshareable");
    }

    target.file = this->file;
    file->markUnshareable();
}


// Get the file name
std::string FileManager::getFileName() const {
    return namefile;
}

// Validate if the file stream is ready for reading
void FileManager::validateReadStream() const {
    if (!file.operator->() || !file->stream) {
        throw FileException(FileException::ErrorType::ReadError,
                            "Invalid file stream.");
    }
    file->stream->open(this->namefile, std::ios::in);
    if (!file->stream->is_open()) {
        throw FileException(FileException::ErrorType::ReadError,
                            "Unable to open file stream for reading.");
    }
}

// Validate if the file stream is ready for writing
void FileManager::validateWriteStream() {
    if (file->stream->is_open()) {
        file->stream->close();
        file->stream->clear();
    }
    if (!file.operator->()) {
        throw FileException(FileException::ErrorType::WriteError,
                            "Invalid file.");
    }
    if (file->isShared()) {
        file = new FileValue(*file);
    }
    file->markUnshareable();
    file->stream->clear();
    file->stream->open(this->namefile, std::ios::in | std::ios::out);
    if (!file->stream->is_open()) {
        throw FileException(FileException::ErrorType::WriteError,
                            "Unable to open file stream for writing.");
    }
}

// Validate index bounds when accessing the file content
void FileManager::validateIndex(int i) const {
    if (i < 0 || i > count) {
        throw FileException(FileException::ErrorType::ReadError,
                            "Index is out of bounds.");
    }
}


// Copy assignment operator
FileManager& FileManager::operator=(const FileManager& other) {
    if (this != &other) {
        namefile = other.namefile;
        count = other.count;
        file = other.file;
    }
    return *this;
}
