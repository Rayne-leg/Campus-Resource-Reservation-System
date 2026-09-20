#include <iostream>
#include "../include/Reservation.h"
#include "../include/ReservationManager.h"

int main()
{
    ReservationManager manager;

    Reservation r1("RES1", "1001", "Alice Smith",
                   "R205", "2026-09-20");

    Reservation r2("RES2", "1002", "Bob Jones",
                   "R101", "2026-09-21");

    Reservation r3("RES3", "1003", "Charlie Brown",
                   "R310", "2026-09-22");

    manager.addReservation(r1);
    manager.addReservation(r2);
    manager.addReservation(r3);

    std::cout << "ACTIVE RESERVATIONS" << std::endl;
    std::cout << "===================" << std::endl;
    manager.displayReservations();

    std::cout << "\nSearching for RES2..." << std::endl;

    if (manager.searchReservation("RES2"))
        std::cout << "Reservation found." << std::endl;
    else
        std::cout << "Reservation not found." << std::endl;

    std::cout << "\nChanging RES2 date..." << std::endl;

    if (manager.editReservation("RES2", "2026-09-30"))
        std::cout << "Reservation updated." << std::endl;
    else
        std::cout << "Reservation not found or date conflict." << std::endl;

    Reservation removed;

    std::cout << "\nRemoving RES2..." << std::endl;

    if (manager.removeReservation("RES2", removed))
    {
        std::cout << "Reservation removed." << std::endl;
        std::cout << "Removed Reservation: "
                  << removed.getReservationID() << std::endl;
        std::cout << "Student: "
                  << removed.getStudentName() << std::endl;
        std::cout << "Resource: "
                  << removed.getResourceID() << std::endl;
    }
    else
    {
        std::cout << "Reservation not found." << std::endl;
    }

    std::cout << "\nACTIVE RESERVATIONS AFTER REMOVAL" << std::endl;
    std::cout << "=================================" << std::endl;

    manager.displayReservations();

    return 0;
}