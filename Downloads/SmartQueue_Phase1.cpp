#include <iostream>
using namespace std;

#pragma once
#ifndef _NODE
#define _NODE
template <typename T>
class Node {
private:
    T item;
    Node<T>* next;
public:
    Node();
    Node(const T& r_Item);
    void setItem(const T& r_Item);
    void setNext(Node<T>* nextNodePtr);
    T getItem() const;
    Node<T>* getNext() const;
};
#endif

template <typename T>
Node<T>::Node() { next = nullptr; }

template <typename T>
Node<T>::Node(const T& r_Item) { item = r_Item; next = nullptr; }

template <typename T>
void Node<T>::setItem(const T& r_Item) { item = r_Item; }

template <typename T>
void Node<T>::setNext(Node<T>* nextNodePtr) { next = nextNodePtr; }

template <typename T>
T Node<T>::getItem() const { return item; }

template <typename T>
Node<T>* Node<T>::getNext() const { return next; }

template<typename T>
class LinkedListQueue {
private:
    Node<T>* backPtr;
    Node<T>* frontPtr;
public:
    LinkedListQueue();
    bool isEmpty() const;
    bool enqueue(const T& newEntry);
    bool dequeue(T& TopEntry);
    bool peek(T& TopEntry) const;
    ~LinkedListQueue();
};

template<typename T>
LinkedListQueue<T>::LinkedListQueue() {
    frontPtr = backPtr = nullptr;
}

template<typename T>
bool LinkedListQueue<T>::isEmpty() const {
    return frontPtr == nullptr;
}

template<typename T>
bool LinkedListQueue<T>::enqueue(const T& newEntry) {
    Node<T>* newNode = new Node<T>(newEntry);
    if (isEmpty()) {
        frontPtr = backPtr = newNode;
    } else {
        backPtr->setNext(newNode);
        backPtr = newNode;
    }
    return true;
}

template<typename T>
bool LinkedListQueue<T>::dequeue(T& TopEntry) {
    if (isEmpty())
        return false;
    Node<T>* nodeToDelete = frontPtr;
    TopEntry = nodeToDelete->getItem();
    frontPtr = frontPtr->getNext();
    if (frontPtr == nullptr)
        backPtr = nullptr;
    delete nodeToDelete;
    return true;
}

template<typename T>
bool LinkedListQueue<T>::peek(T& TopEntry) const {
    if (isEmpty())
        return false;
    TopEntry = frontPtr->getItem();
    return true;
}

template<typename T>
LinkedListQueue<T>::~LinkedListQueue() {
    T temp;
    while (dequeue(temp));
}

// Utility functions

template <typename T>
void PrintQueue(LinkedListQueue<T> Q) {
    T item;
    cout << "Queue contents: ";
    while (Q.dequeue(item)) {
        cout << item << " ";
    }
    cout << endl;
}

template <typename T>
int QueueSum(LinkedListQueue<int> Q) {
    int item, sum = 0;
    while (Q.dequeue(item)) {
        sum += item;
    }
    return sum;
}

template <typename T>
void removeFirstPostitive(LinkedListQueue<T>& Q) {
    T item;
    while (!Q.isEmpty()) {
        Q.peek(item);
        if (item == 0)
            break;
        Q.dequeue(item);
    }
}

int main() {
    LinkedListQueue<int> queue;

    if (queue.isEmpty()) {
        cout << "Queue is initially empty.\n";
    }

    cout << "Enqueuing numbers 10, 20, 30 into the queue.\n";
    queue.enqueue(10);
    queue.enqueue(20);
    queue.enqueue(30);

    int frontItem;
    if (queue.peek(frontItem)) {
        cout << "Front item after enqueues: " << frontItem << "\n";
    }

    cout << "Dequeuing items:\n";
    while (!queue.isEmpty()) {
        if (queue.dequeue(frontItem)) {
            cout << "Dequeued: " << frontItem << "\n";
        }
    }

    if (queue.isEmpty()) {
        cout << "Queue is now empty after dequeuing all items.\n";
    }

    return 0;
}
