#include "Proxy.h"
#include "FileManager.h"

// Constructor initializes Proxy with a FileManager pointer and an index
Proxy::Proxy(const FileManager* file,int idx) : f(const_cast<FileManager*>(file)),index(idx) {
}

// Conversion operator to return a character from the file at the specified index
Proxy::operator char() const {
    f->file->stream->open(f->namefile,std::ios::in | std::ios::out | std::ios::binary);
    f->file->stream->clear();  // Clear any previous error states
    f->file->stream->seekg(index,std::ios::beg);  // Move the read pointer to the given index
    char c;
    f->file->stream->get(c);  // Read the character from the file
    f->file->stream->flush();
    f->file->stream->close();
    f->file->stream->clear();
    return c;
}

// Assignment operator to set the character at the specified index in the file
Proxy& Proxy::operator=(char c) {
    f->file->stream->open(f->namefile, std::ios::in | std::ios::out | std::ios::binary);
    f->file->stream->clear();
    f->file->stream->seekp(index, std::ios::beg);  // Move the write pointer to the given index
    f->file->stream->put(c);

    // If the index exceeds the current file size, increment the file count
    if (index >= f->count)
        f->count++;

    f->file->stream->flush();
    f->file->stream->close();
    f->file->stream->clear();
    return *this;
}
