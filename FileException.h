#ifndef EX1_FILEEXCEPTION_H
#define EX1_FILEEXCEPTION_H

#include <stdexcept>
#include <string>
#include <utility>

//class for file-errors
class FileException : public std::exception {
public:

    enum ErrorType {
        NotOpen,       // Error: file not open
        FileNotFound,  // Error: file not found
        ReadError,     // Error: failed to read from file
        WriteError,    // Error: failed to write to file
        CopyError,     // Error: failed to copy file
        DeleteError    // Error: failed to delete file
    };

private:
    __attribute__((unused)) ErrorType type;        // The specific error type
    std::string message;   // Error message to display

public:
    // Constructor: initializes the error type and message
    FileException(ErrorType type, std::string  msg) : type(type), message(std::move(msg)) {}

    // Override the what() function to return the error message
    const char* what() const noexcept override { return message.c_str(); }


};

#endif // EX1_FILEEXCEPTION_H
