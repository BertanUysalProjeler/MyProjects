#include "Stack.h"
#include <iostream>

// Constructor to initialize an empty stack
Stack::Stack() : top(-1) {
    // Initialize the stack to an empty state
    for (int i = 0; i < MAX_SIZE; ++i) {
        data[i] = 0; // Optional: Clear the stack elements to zero
    }
}

// Adds a province to the top of the stack
void Stack::push(int province) {
    if (top == MAX_SIZE -1)
    {
        return;
    }
    data[++top] = province;
}

// Removes and returns the top province from the stack
int Stack::pop() {
    if (top == -1) {
        return -1; // Return an error code for empty stack
    }
    return data[top--]; // Return the top element and decrement the top index
}

// Returns the top province without removing it
int Stack::peek() const {
    if (data[top] == 0)
    {
        return -1;
    }
    return data[top];
}

// Checks if the stack is empty
bool Stack::isEmpty() const {
    return top == -1; // Return true if stack is empty
}

// Function to get the current size of the stack
int Stack::getSize() const {
    return top + 1; // Size is one more than the top index
}
