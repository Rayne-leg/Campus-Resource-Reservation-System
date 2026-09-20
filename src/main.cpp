#include <iostream>
#include "../include/Reservation.h"
#include "../include/ReservationManager.h"

int main()
{
    ReservationManager manager;

    Reservation r1(301, 1001, "Alice Smith", "R205", "09/20/2026");
    Reservation r2(302, 1002, "Bob Jones", "R101", "09/21/2026");
    Reservation r3(303, 1003, "Charlie Brown", "R310", "09/22/2026");

    manager.addReservation(r1);
    manager.addReservation(r2);
    manager.addReservation(r3);

    std::cout << "ACTIVE RESERVATIONS" << std::endl;
    std::cout << "===================" << std::endl;

    manager.displayReservations();

    std::cout << "\nSearching for reservation 302..." << std::endl;

    if (manager.searchReservation(302))
    {
        std::cout << "Reservation found." << std::endl;
    }
    else
    {
        std::cout << "Reservation not found." << std::endl;
    }

    std::cout << "\nCancelling reservation 302..." << std::endl;

    if (manager.cancelReservation(302))
    {
        std::cout << "Reservation cancelled." << std::endl;
    }
    else
    {
        std::cout << "Reservation not found." << std::endl;
    }

    std::cout << "\nACTIVE RESERVATIONS AFTER CANCELLATION" << std::endl;
    std::cout << "======================================" << std::endl;

    manager.displayReservations();

    return 0;
}