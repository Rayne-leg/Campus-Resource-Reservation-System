#ifndef RESERVATION_MANAGER_H
#define RESERVATION_MANAGER_H

#include "Reservation.h"

class ReservationManager
{
private:
    struct Node
    {
        Reservation data;
        Node* next;
    };

    Node* head;

public:
    // Constructor
    ReservationManager();

    // Destructor
    ~ReservationManager();

    // Add a reservation
    void addReservation(Reservation reservation);

    // Display all active reservations
    void displayReservations() const;

    // Search for a reservation by ID
    bool searchReservation(int reservationID) const;

    // Cancel/remove a reservation by ID
    bool cancelReservation(int reservationID);
};

#endif