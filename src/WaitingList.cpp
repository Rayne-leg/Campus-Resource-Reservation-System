#include "WaitingList.h"

#include <iostream>

WaitingRequest::WaitingRequest()
{
    studentID = "";
    studentName = "";
    resourceID = "";
    reservationDate = "";
}

WaitingRequest::WaitingRequest(std::string id,
                               std::string name,
                               std::string resource,
                               std::string date)
{
    studentID = id;
    studentName = name;
    resourceID = resource;
    reservationDate = date;
}

WaitingList::WaitingList()
{
    front = nullptr;
    rear = nullptr;
}

WaitingList::~WaitingList()
{
    while (front != nullptr)
    {
        Node* temp = front;
        front = front->next;
        delete temp;
    }

    rear = nullptr;
}

void WaitingList::enqueue(const WaitingRequest& request)
{
    Node* newNode = new Node(request);

    if (rear == nullptr)
    {
        front = newNode;
        rear = newNode;
        return;
    }

    rear->next = newNode;
    rear = newNode;
}

bool WaitingList::dequeue(WaitingRequest& removedRequest)
{
    if (front == nullptr)
    {
        std::cout << "Waiting list is empty." << std::endl;
        return false;
    }

    Node* temp = front;

    removedRequest = front->request;

    front = front->next;

    if (front == nullptr)
    {
        rear = nullptr;
    }

    delete temp;

    return true;
}

void WaitingList::displayWaitingList() const
{
    if (front == nullptr)
    {
        std::cout << "Waiting list is empty." << std::endl;
        return;
    }

    Node* current = front;

    std::cout << "\n===== WAITING LIST =====" << std::endl;

    while (current != nullptr)
    {
        std::cout << "Student ID: "
                  << current->request.studentID
                  << std::endl;

        std::cout << "Student Name: "
                  << current->request.studentName
                  << std::endl;

        std::cout << "Resource ID: "
                  << current->request.resourceID
                  << std::endl;

        std::cout << "Reservation Date: "
                  << current->request.reservationDate
                  << std::endl;

        std::cout << "------------------------"
                  << std::endl;

        current = current->next;
    }
}

bool WaitingList::isEmpty() const
{
    return front == nullptr;
} 