#include <iostream>
#include "../include/ReservationManager.h"

ReservationManager::ReservationManager()
{
    head = nullptr;
}
void ReservationManager::addReservation(Reservation reservation)
{
    Node* newNode = new Node;

    newNode->data = reservation;
    newNode->next = head;
    head = newNode;
}
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
