#include "../include/ReservationManager.h"
#include <iostream>

// Constructor
ReservationManager::ReservationManager()
{
    head = nullptr;
}

// Destructor
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

// Add a reservation to the linked list
void ReservationManager::addReservation(Reservation reservation)
{
    Node* newNode = new Node;

    newNode->data = reservation;
    newNode->next = head;

    head = newNode;
}

// Display all active reservations
void ReservationManager::displayReservations() const
{
    Node* current = head;

    while (current != nullptr)
    {
        std::cout << "Reservation ID: "
                  << current->data.getReservationID() << std::endl;

        std::cout << "Student ID: "
                  << current->data.getStudentID() << std::endl;

        std::cout << "Student Name: "
                  << current->data.getStudentName() << std::endl;

        std::cout << "Resource ID: "
                  << current->data.getResourceID() << std::endl;

        std::cout << "Reservation Date: "
                  << current->data.getReservationDate() << std::endl;

        std::cout << "------------------------" << std::endl;

        current = current->next;
    }
}

// Search for a reservation by reservation ID
bool ReservationManager::searchReservation(int reservationID) const
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

// Cancel/remove a reservation by reservation ID
bool ReservationManager::cancelReservation(int reservationID)
{
    Node* current = head;
    Node* previous = nullptr;

    while (current != nullptr)
    {
        if (current->data.getReservationID() == reservationID)
        {
            // Reservation is the first node
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