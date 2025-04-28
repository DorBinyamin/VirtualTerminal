#include "Folder.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <functional>

// Global current working directory pointer
static Folder* current = nullptr;

// Utility to split a string by a delimiter
static std::vector<std::string> splitInternal(const std::string& s, char delim = '#') {
    std::vector<std::string> parts;
    std::string token;
    std::istringstream iss(s);
    while (std::getline(iss, token, delim)) {
        if (!token.empty()) parts.push_back(token);
    }
    return parts;
}

// Folder constructor
Folder::Folder(const char* name)
        : foldername(name ? name : ""), parent(nullptr) {
    // Initialize current to root on first construction
    if (!current) current = this;
}
// Folder destructor
Folder::~Folder() {
    // Recursively remove all files on disk and clear subfolders
    std::function<void(Folder*)> clearAll = [&](Folder* f) {
        for (auto& fm : f->files) {
            fm.remove(fm.getFileName().c_str());
        }
        for (auto& sf : f->subfolders) {
            clearAll(&sf);
        }
        f->files.clear();
        f->subfolders.clear();
    };
    clearAll(this);
    if (current == this) current = nullptr;
}
// Create a new folder
void Folder::mkdir(const char* name) {
    if (!name || std::string(name).empty()) {
        std::cerr << "mkdir: missing folder name" << std::endl;
        return;
    }
    auto path = splitInternal(name, '/');
    Folder* node = path.empty() || path[0] != foldername ? current : this;
    if (!path.empty() && path[0] == foldername) path.erase(path.begin());
    for (size_t i = 0; i < path.size(); ++i) {
        const auto& part = path[i];
        auto it = std::find_if(node->subfolders.begin(), node->subfolders.end(),
                               [&](const Folder& f) { return f.foldername == part; });
        if (it == node->subfolders.end()) {
            if (i + 1 < path.size()) {
                std::cerr << "mkdir: cannot create folder '" << part
                          << "' because parent folder does not exist" << std::endl;
                return;
            }
            node->subfolders.emplace_back(part.c_str());
            node->subfolders.back().parent = node;
            node = &node->subfolders.back();
        } else {
            if (i + 1 == path.size()) {
                std::cerr << "mkdir: folder '" << part
                          << "' already exists at thiscout level" << std::endl;
                return;
            }
            node = &*it;
        }
    }
}
// Change current working directory
void Folder::chdir(const char* name) {
    if (!name || std::string(name).empty()) {
        std::cerr << "chdir: missing folder name" << std::endl;
        return;
    }
    auto path = splitInternal(name, '/');
    Folder* node = path.empty() || path[0] != foldername ? current : this;
    if (!path.empty() && path[0] == foldername) path.erase(path.begin());
    for (const auto& part : path) {
        if (part == "..") {
            if (node->parent) node = node->parent;
        } else {
            auto it = std::find_if(node->subfolders.begin(), node->subfolders.end(),
                                   [&](const Folder& f) { return f.foldername == part; });
            if (it == node->subfolders.end()) {
                std::cerr << "folder '" << part << "' not found" << std::endl;
                return;
            }
            node = &*it;
        }
    }
    current = node;
}
// Remove a folder
void Folder::rmdir(const char* name) {
    if (!name || std::string(name).empty()) {
        std::cerr<< "folder name is empty" << std::endl;
        return;
    }
    auto path = splitInternal(name, '/');
    Folder* node = path.empty() || path[0] != foldername ? current : this;
    if (!path.empty() && path[0] == foldername) path.erase(path.begin());
    for (const auto& part : path) {
        auto it = std::find_if(node->subfolders.begin(), node->subfolders.end(),
                               [&](const Folder& f) { return f.foldername == part; });
        if (it == node->subfolders.end()) {
            std::cerr << "folder '" << part << "' not found" << std::endl;
            return;
        }
        node = &*it;
    }
    if (node == current && !node->parent) {
        std::cerr << "cannot remove root folder" << std::endl;
        return;
    }
    // Recursively remove all files and subfolders
    std::function<void(Folder*)> clearAll = [&](Folder* f) {
        for (auto& fm : f->files) fm.remove(fm.getFileName().c_str());
        for (auto& sf : f->subfolders) clearAll(&sf);
        f->files.clear();
        f->subfolders.clear();
    };
    clearAll(node);
    if (current == node && node->parent) current = node->parent;
    auto parentPtr = node->parent;
    parentPtr->subfolders.erase(
            std::remove_if(parentPtr->subfolders.begin(), parentPtr->subfolders.end(),
                           [&](const Folder& f) { return f.foldername == node->foldername; }),
            parentPtr->subfolders.end());
}

// show folder contents
void Folder::ls(const char* name) const {
    const Folder* node = name && splitInternal(name, '/')[0] == foldername ? this : current;
    if (name && !std::string(name).empty()) {
        auto path = splitInternal(name, '/');
        if (path[0] == foldername) path.erase(path.begin());
        for (const auto& part : path) {
            auto it = std::find_if(node->subfolders.begin(), node->subfolders.end(),
                                   [&](const Folder& f) { return f.foldername == part; });
            if (it == node->subfolders.end()) {
                std::cerr << "folder '" << part << "' not found" << std::endl;
                return;
            }
            node = &*it;
        }
    }
    std::vector<std::string> full;
    for (const Folder* tmp = node; tmp; tmp = tmp->parent) full.push_back(tmp->foldername);
    std::reverse(full.begin(), full.end());
    for (const auto& p : full) std::cout << p << "/";
    std::cout << std::endl;
    for (const auto& sf : node->subfolders) std::cout << sf.foldername << "/" << std::endl;
    for (const auto& fm : node->files) {
        auto parts = splitInternal(fm.getFileName(), '#');
        std::cout << parts.back() << "std::endl";
    }
}

//folder tree starting from root
void Folder::lproot() const {
    std::function<void(const Folder*, int)> printTree;
    printTree = [&](const Folder* f, int indent) {
        std::cout << std::string(indent, ' ') << f->foldername << "/" << std::endl;
        for (const auto& fm : f->files) {
            auto parts = splitInternal(fm.getFileName(), '#');
            int refc = fm.getRefCount();
            std::cout << std::string(indent + 4, ' ') << parts.back() << " " << refc <<std::endl;
        }
        for (const auto& sf : f->subfolders) printTree(&sf, indent + 4);
    };
    printTree(this, 0);
}

// Print current working directory path
void Folder::pwd() {
    std::vector<std::string> parts;
    for (const Folder* tmp = current; tmp; tmp = tmp->parent) parts.push_back(tmp->foldername);
    for (auto it = parts.rbegin(); it != parts.rend(); ++it) std::cout << *it << "/";
    std::cout << std::endl;
}

// Add a file into the folder
void Folder::addFile(const FileManager& fm) {
    auto parts = splitInternal(fm.getFileName(), '#');
    if (parts.size() < 2) { std::cerr << "invalid file path" << std::endl; return; }
    parts.pop_back();
    Folder* node = parts[0] == foldername ? this : current;
    if (!parts.empty() && parts[0] == foldername) parts.erase(parts.begin());
    for (const auto& part : parts) {
        auto it = std::find_if(node->subfolders.begin(), node->subfolders.end(),
                               [&](const Folder& f) { return f.foldername == part; });
        if (it == node->subfolders.end()) { std::cerr << "folder '" << part << "' not found" <<std::endl; return; }
        node = &*it;
    }
    node->files.emplace_back(fm);

}

// Get a pointer to a file by name
FileManager* Folder::getFile(const std::string& name) {
    auto it = std::find_if(files.begin(), files.end(),
                           [&](const FileManager& fm) { return fm.getFileName() == name; });
    if (it != files.end()) {
        return &(*it);
    }

    for (auto& sub : subfolders) {
        FileManager* found = sub.getFile(name);
        if (found != nullptr) {
            return found;
        }
    }
    return nullptr;
}

// Remove a file given its full path
void Folder::removeFile(const std::string& fullPath) {
    auto parts = splitInternal(fullPath, '#');
    if (parts.size() < 2) { std::cerr << "invalid file path" << std::endl; return; }
    parts.pop_back();
    Folder* node = parts.empty() || parts[0] != foldername ? current : this;
    if (!parts.empty() && parts[0] == foldername) parts.erase(parts.begin());
    for (const auto& part : parts) {
        auto it = std::find_if(node->subfolders.begin(), node->subfolders.end(),
                               [&](const Folder& f) { return f.foldername == part; });
        if (it == node->subfolders.end()) { std::cerr << "folder '" << part << "' not found" << std::endl; return; }
        node = &*it;
    }
    auto itf = std::find_if(node->files.begin(), node->files.end(),
                            [&](const FileManager& fm) { return fm.getFileName() == fullPath; });
    if (itf == node->files.end()) { std::cerr << "file '" << fullPath << "' not found" << std::endl; return; }
    itf->remove(fullPath.c_str());
    node->files.erase(itf);
}

//check if folder Exist
bool Folder::folderExists(const std::string& fullPath) const {
    auto parts = splitInternal(fullPath, '#');
    if (parts.size() < 2) return false;
    parts.pop_back();
    const Folder* node = (parts[0] == foldername ? this : current);
    if (parts[0] == foldername) parts.erase(parts.begin());

    for (const auto& part : parts) {
        auto it = std::find_if(node->subfolders.begin(), node->subfolders.end(),
                               [&](const Folder& f) { return f.foldername == part; });
        if (it == node->subfolders.end())
            return false;
        node = &*it;
    }
    return true;
}

