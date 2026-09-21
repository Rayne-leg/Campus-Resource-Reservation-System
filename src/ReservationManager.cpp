#include "../include/ReservationManager.h"
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

bool ReservationManager::addReservation(const Reservation& reservation)
{
    // Check for duplicate reservation ID
    if (searchReservation(reservation.getReservationID()))
    {
        return false;
    }

    // Check if the same resource is already reserved on the same date
    if (hasResourceDateConflict(reservation.getResourceID(),
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
        std::cout << "No active reservations." << std::endl;
        return;
    }

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
            // Save reservation before deleting the node
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
            // Check that changing the date will not create
            // a resource/date conflict with another reservation
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