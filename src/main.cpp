#include <iostream>
#include "../include/Reservation.h"
#include "../include/ReservationManager.h"

int main()
{
    Reservation r1(301, 1001, "Alice Smith", "R205", "09/20/2026");
    Reservation r2(302, 1002, "Bob Jones", "R101", "09/21/2026");
    Reservation r3(303, 1003, "Charlie Brown", "R310", "09/22/2026");

    ReservationManager manager;

    manager.addReservation(r1);
    manager.addReservation(r2);
    manager.addReservation(r3);

    manager.displayReservations();

    return 0;
}