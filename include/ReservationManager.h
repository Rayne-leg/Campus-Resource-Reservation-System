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

    static Node* mergeSort(Node* start);

    static Node* mergeSortedLists(
        Node* left, Node* right);

public:
    ReservationManager();
    ~ReservationManager();

    bool addReservation(const Reservation& reservation);

    void displayReservations() const;

    bool searchReservation(
        const std::string& reservationID) const;

    bool hasResourceDateConflict(
        const std::string& resourceID,
        const std::string& reservationDate) const;

    bool removeReservation(
        const std::string& reservationID,
        Reservation& removedReservation);

    bool editReservation(
        const std::string& reservationID,
        const std::string& newDate);

    int countReservationsForResource(
        const std::string& resourceID) const;

    void sortByDate();
};

#endif