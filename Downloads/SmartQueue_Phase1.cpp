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
    }
    else {
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

// Test case demonstration
int main() {
    LinkedListQueue<int> q;

    cout << "Is queue empty? " << (q.isEmpty() ? "Yes" : "No") << endl;

    cout << "Enqueuing 10, 20, 30..." << endl;
    q.enqueue(10);
    q.enqueue(20);
    q.enqueue(30);

    int frontItem;
    if (q.peek(frontItem)) {
        cout << "Front item: " << frontItem << endl;
    }

    cout << "Dequeuing all items:" << endl;
    while (!q.isEmpty()) {
        if (q.dequeue(frontItem)) {
            cout << "Dequeued: " << frontItem << endl;
        }
    }

    cout << "Is queue empty after dequeue? " << (q.isEmpty() ? "Yes" : "No") << endl;

    return 0;
}
