#include "Queue.h"
#include <iostream>

Queue::Queue() {
    front = rear = -1; // Initialize an empty queue
}

// Adds a province to the end of the queue
void Queue::enqueue(int province) {
    if ((rear + 1) % MAX_QUEUE_SIZE == front) {
        return;
    }

    if (front == -1) { // Queue is empty
        front = rear = 0;
    } else {
        rear = (rear + 1) % MAX_QUEUE_SIZE;
    }
    data[rear] = province;
}

// Removes and returns the front province from the queue
int Queue::dequeue() {
    if (this->isEmpty()) {
        return -1;
    }
    int carrier = data[front]; // Fetch the value at the front
    if (front == rear) { // Only one element left
        front = rear = -1; // Reset the queue to empty
    } else {
        front = (front + 1) % MAX_QUEUE_SIZE;
    }
    return carrier;
}

// Returns the front province without removing it
int Queue::peek() const {
    if (this->isEmpty()) {
        return -1;
    }
    return data[front];
}

// Checks if the queue is empty
bool Queue::isEmpty() const {
    return (front == -1);
}

// Add a priority neighboring province in a way that it is dequeued before others
void Queue::enqueuePriority(int province) {
    if ((rear + 1) % MAX_QUEUE_SIZE == front) {
        return;
    }

    if (front == -1) { // Queue is empty
        front = rear = 0;
    } else {
        front = (front - 1 + MAX_QUEUE_SIZE) % MAX_QUEUE_SIZE;
    }
    data[front] = province;
}

void Queue::clear() {
    while (!isEmpty()) {
        dequeue();
    }
}
