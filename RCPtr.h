#ifndef EX1_RCPTR_H
#define EX1_RCPTR_H

// Template class representing a reference-counted smart pointer
template<class T>
class RCPtr
{
private:
    T* pointee;  // Pointer to the managed object (the actual object being pointed to)

    // Initializes the pointer by adding a reference to the object
    void init();

public:
    // Constructor that takes a raw pointer (defaults to nullptr)
    RCPtr(T* realPtr = nullptr) : pointee(realPtr) { init(); }

    // Copy constructor, shares the same pointee and manages the reference count
    RCPtr(const RCPtr& rhs) : pointee(rhs.pointee) { init(); }

    // Destructor, removes reference from the pointee if it's not null
    ~RCPtr() { if (pointee) pointee->removeReference(); }

    // Assignment operator that manages reference count correctly
    RCPtr& operator=(const RCPtr& rhs);

    // Access the pointee using -> operator
    T* operator->() const { return pointee; }

    // Dereference operator to access the pointee's value
    T& operator*() const { return *pointee; }
};

// Initializes the pointee, handles reference counting and object sharing
template<class T>
void RCPtr<T>::init()
{
    if (pointee == nullptr) return;  // If the pointer is null, no need to do anything

    // If the pointee is not shareable, make a copy of it to ensure it can be shared
    if (pointee->isShareable() == false) {
        pointee = new T(*pointee);  // Requires a copy constructor for T
    }

    // Adds a reference to the pointee
    pointee->addReference();
}

// Assignment operator, ensures correct reference counting when assigning one RCPtr to another
template<class T>
RCPtr<T>& RCPtr<T>::operator=(const RCPtr<T>& rhs)
{
    if (pointee != rhs.pointee) {
        T* oldPointee = pointee;
        pointee = rhs.pointee;
        init();  // Initializes the new pointee
        if (oldPointee)
            oldPointee->removeReference();  // Removes reference from the old pointee
    }
    return *this;
}

#endif //EX1_RCPTR_H
