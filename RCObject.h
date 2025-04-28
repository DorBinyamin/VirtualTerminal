#ifndef EX1_RCOBJECT_H
#define EX1_RCOBJECT_H

// Class representing a reference-counted object
class RCObject {
protected:
    // Constructor initializes reference count to 0 and marks object as shareable
    RCObject() : refCount(0), shareable(true) {}

    // Copy constructor initializes reference count to 0 and marks object as shareable
    RCObject(const RCObject&) : refCount(0), shareable(true) {}

    // Assignment operator (does nothing in this case)
    RCObject& operator=(const RCObject&);

    // Virtual destructor, defined as pure virtual to ensure it is overridden
    virtual ~RCObject() = 0;

public:
    // Increments the reference count when a new reference to the object is made
    void addReference();

    // Decrements the reference count and deletes the object if the count reaches zero
    void removeReference();

    // Marks the object as unshareable (it cannot be shared anymore)
    void markUnshareable();

    // Returns true if the object is shareable (can be referenced by more than one owner)
    bool isShareable() const;

    // Returns true if the object is shared (reference count > 1)
    bool isShared() const;

    // Returns the current reference count
    int getRefCount() const { return refCount; }

private:
    int refCount;  // Keeps track of the number of references to this object
    bool shareable;  // Indicates whether the object can be shared
};

#endif //EX1_RCOBJECT_H
