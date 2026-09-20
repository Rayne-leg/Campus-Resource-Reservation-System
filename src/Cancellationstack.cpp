#include "CancellationStack.h"

#include <iostream>

CancelledReservation::CancelledReservation()
{
    reservationID = "";
    studentID = "";
    studentName = "";
    resourceID = "";
    reservationDate = "";
    cancelDate = "";
}

CancelledReservation::CancelledReservation(std::string resID,
                                            std::string studID,
                                            std::string studName,
                                            std::string resourceId,
                                            std::string resDate,
                                            std::string cancelledOn)
{
    reservationID = resID;
    studentID = studID;
    studentName = studName;
    resourceID = resourceId;
    reservationDate = resDate;
    cancelDate = cancelledOn;
}

CancellationStack::CancellationStack()
{
    top = nullptr;
    count = 0;
}

CancellationStack::~CancellationStack()
{
    while (top != nullptr)
    {
        Node* temp = top;
        top = top->next;
        delete temp;
    }
}

void CancellationStack::push(const CancelledReservation& record)
{
    Node* newNode = new Node(record);

    newNode->next = top;
    top = newNode;

    count++;
}

bool CancellationStack::pop(CancelledReservation& removedRecord)
{
    if (top == nullptr)
    {
        std::cout << "Cancellation history is empty." << std::endl;
        return false;
    }

    Node* temp = top;

    removedRecord = top->record;

    top = top->next;

    delete temp;

    count--;

    return true;
}

bool CancellationStack::peek(CancelledReservation& record) const
{
    if (top == nullptr)
    {
        return false;
    }

    record = top->record;

    return true;
}

void CancellationStack::displayHistory() const
{
    if (top == nullptr)
    {
        std::cout << "No cancellations have been made." << std::endl;
        return;
    }

    Node* current = top;

    std::cout << "\n===== CANCELLATION HISTORY (most recent first) =====" << std::endl;

    while (current != nullptr)
    {
        std::cout << "Reservation ID: " << current->record.reservationID << std::endl;
        std::cout << "Student ID: " << current->record.studentID << std::endl;
        std::cout << "Student Name: " << current->record.studentName << std::endl;
        std::cout << "Resource ID: " << current->record.resourceID << std::endl;
        std::cout << "Original Date: " << current->record.reservationDate << std::endl;
        std::cout << "Cancelled On: " << current->record.cancelDate << std::endl;
        std::cout << "------------------------" << std::endl;

        current = current->next;
    }
}

bool CancellationStack::isEmpty() const
{
    return top == nullptr;
}

int CancellationStack::getSize() const
{
    return count;
}
