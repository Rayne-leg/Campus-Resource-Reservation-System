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
   ReservationManager();
   void addReservation(Reservation reservation);
   void displayReservations() const;

};

#endif