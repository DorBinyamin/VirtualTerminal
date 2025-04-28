#include "RCObject.h"

// Assignment operator does nothing, simply returns the current object (no-op)
RCObject& RCObject::operator=(const RCObject&) {
    return *this;
}

// Increments the reference count when a new reference to the object is made
void RCObject::addReference(){
    ++refCount;
}

// Decrements the reference count and deletes the object if the count reaches zero
void RCObject::removeReference(){
    if(--refCount == 0) delete this;
}

// Marks the object as unshareable, meaning it can no longer be shared
void RCObject::markUnshareable(){
    shareable = false;
}

// Returns true if the object is shareable (can be referenced by more than one owner)
bool RCObject::isShareable() const {
    return shareable;
}

// Default destructor does nothing (necessary for polymorphic behavior)
RCObject::~RCObject() = default;

// Returns true if the object is shared (more than one reference exists)
bool RCObject::isShared() const {
    return refCount > 1;
}
