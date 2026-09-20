#ifndef RESERVATION_MANAGER_H
#define RESERVATION_MANAGER_H

#include "Reservation.h"
#include <string>

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
    ~ReservationManager();

    bool addReservation(const Reservation& reservation);

    void displayReservations() const;

    bool searchReservation(const std::string& reservationID) const;

    bool removeReservation(const std::string& reservationID,
                           Reservation& removedReservation);

    bool editReservation(const std::string& reservationID,
                         const std::string& newDate);
};

#endif