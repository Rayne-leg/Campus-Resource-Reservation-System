#include "WaitingList.h"

#include <iostream>

bool WaitingList::peekForResourceDate(const std::string& resourceID,
                                     const std::string& date,
                                     WaitingRequest& request) const
{
    for (Node* current = front; current != nullptr; current = current->next)
    {
        if (current->request.resourceID == resourceID &&
            current->request.reservationDate == date)
        {
            request = current->request;
            return true;
        }
    }
    return false;
}

bool WaitingList::dequeueForResourceDate(
    const std::string& resourceID,
    const std::string& date,
    WaitingRequest& removedRequest)
{
    Node* current = front;
    Node* previous = nullptr;
    while (current != nullptr)
    {
        if (current->request.resourceID == resourceID &&
            current->request.reservationDate == date)
        {
            removedRequest = current->request;
            if (previous == nullptr)
                front = current->next;
            else
                previous->next = current->next;

            if (current == rear)
                rear = previous;

            delete current;
            return true;
        }
        previous = current;
        current = current->next;
    }
    return false;
}

int WaitingList::countWaitingRequestsForResource(
    const std::string& resourceID) const
{
    int count = 0;
    for (Node* current = front; current != nullptr; current = current->next)
        if (current->request.resourceID == resourceID)
            ++count;
    return count;
}

int WaitingList::countWaitingStudentsForResource(
    const std::string& resourceID) const
{
    int count = 0;
    for (Node* current = front; current != nullptr; current = current->next)
    {
        if (current->request.resourceID != resourceID)
            continue;

        bool appearedEarlier = false;
        for (Node* earlier = front; earlier != current; earlier = earlier->next)
        {
            if (earlier->request.resourceID == resourceID &&
                earlier->request.studentID == current->request.studentID)
            {
                appearedEarlier = true;
                break;
            }
        }
        if (!appearedEarlier)
            ++count;
    }
    return count;
}

bool WaitingList::hasWaitingRequest(const std::string& studentID,
                                    const std::string& resourceID,
                                    const std::string& date) const
{
    for (Node* current = front; current != nullptr; current = current->next)
        if (current->request.studentID == studentID &&
            current->request.resourceID == resourceID &&
            current->request.reservationDate == date)
            return true;
    return false;
}

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
