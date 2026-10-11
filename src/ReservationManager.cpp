#include "ReservationManager.h"
#include <iostream>

ReservationManager::ReservationManager()
{
    head = nullptr;
}

ReservationManager::~ReservationManager()
{
    Node* current = head;

    while (current != nullptr)
    {
        Node* temp = current;
        current = current->next;
        delete temp;
    }
}

bool ReservationManager::addReservation(
    const Reservation& reservation)
{
    if (searchReservation(reservation.getReservationID()))
    {
        return false;
    }

    if (hasResourceDateConflict(
            reservation.getResourceID(),
            reservation.getReservationDate()))
    {
        return false;
    }

    Node* newNode = new Node;
    newNode->data = reservation;
    newNode->next = head;
    head = newNode;

    return true;
}

void ReservationManager::displayReservations() const
{
    if (head == nullptr)
    {
        std::cout << "No active reservations.\n";
        return;
    }

    Node* current = head;

    while (current != nullptr)
    {
        std::cout << "Reservation ID: "
                  << current->data.getReservationID() << '\n';

        std::cout << "Student ID: "
                  << current->data.getStudentID() << '\n';

        std::cout << "Student Name: "
                  << current->data.getStudentName() << '\n';

        std::cout << "Resource ID: "
                  << current->data.getResourceID() << '\n';

        std::cout << "Reservation Date: "
                  << current->data.getReservationDate() << '\n';

        std::cout << "------------------------\n";

        current = current->next;
    }
}

// Manual linear search: O(n).
bool ReservationManager::searchReservation(
    const std::string& reservationID) const
{
    Node* current = head;

    while (current != nullptr)
    {
        if (current->data.getReservationID() == reservationID)
        {
            return true;
        }

        current = current->next;
    }

    return false;
}

bool ReservationManager::hasResourceDateConflict(
    const std::string& resourceID,
    const std::string& reservationDate) const
{
    Node* current = head;

    while (current != nullptr)
    {
        if (current->data.getResourceID() == resourceID &&
            current->data.getReservationDate() == reservationDate)
        {
            return true;
        }

        current = current->next;
    }

    return false;
}

bool ReservationManager::removeReservation(
    const std::string& reservationID,
    Reservation& removedReservation)
{
    Node* current = head;
    Node* previous = nullptr;

    while (current != nullptr)
    {
        if (current->data.getReservationID() == reservationID)
        {
            removedReservation = current->data;

            if (previous == nullptr)
            {
                head = current->next;
            }
            else
            {
                previous->next = current->next;
            }

            delete current;
            return true;
        }

        previous = current;
        current = current->next;
    }

    return false;
}

bool ReservationManager::editReservation(
    const std::string& reservationID,
    const std::string& newDate)
{
    Node* current = head;

    while (current != nullptr)
    {
        if (current->data.getReservationID() == reservationID)
        {
            Node* check = head;

            while (check != nullptr)
            {
                if (check != current &&
                    check->data.getResourceID() ==
                        current->data.getResourceID() &&
                    check->data.getReservationDate() == newDate)
                {
                    return false;
                }

                check = check->next;
            }

            current->data.setReservationDate(newDate);
            return true;
        }

        current = current->next;
    }

    return false;
}

// Counts current active reservations for one resource: O(n).
int ReservationManager::countReservationsForResource(
    const std::string& resourceID) const
{
    int count = 0;
    Node* current = head;

    while (current != nullptr)
    {
        if (current->data.getResourceID() == resourceID)
        {
            count++;
        }

        current = current->next;
    }

    return count;
}

// Combines two lists that are already sorted by date.
ReservationManager::Node* ReservationManager::mergeSortedLists(
    Node* left, Node* right)
{
    Node dummy;
    dummy.next = nullptr;
    Node* tail = &dummy;

    while (left != nullptr && right != nullptr)
    {
        // Taking the left node on equal dates preserves tie order.
        if (left->data.getReservationDate() <=
            right->data.getReservationDate())
        {
            tail->next = left;
            left = left->next;
        }
        else
        {
            tail->next = right;
            right = right->next;
        }

        tail = tail->next;
    }

    if (left != nullptr)
    {
        tail->next = left;
    }
    else
    {
        tail->next = right;
    }

    return dummy.next;
}

// Manual merge sort: O(n log n) time.
ReservationManager::Node* ReservationManager::mergeSort(Node* start)
{
    if (start == nullptr || start->next == nullptr)
    {
        return start;
    }

    Node* slow = start;
    Node* fast = start->next;

    // Find the midpoint.
    while (fast != nullptr && fast->next != nullptr)
    {
        slow = slow->next;
        fast = fast->next->next;
    }

    // Separate the two halves.
    Node* right = slow->next;
    slow->next = nullptr;

    Node* sortedLeft = mergeSort(start);
    Node* sortedRight = mergeSort(right);

    return mergeSortedLists(sortedLeft, sortedRight);
}

void ReservationManager::sortByDate()
{
    head = mergeSort(head);
}

