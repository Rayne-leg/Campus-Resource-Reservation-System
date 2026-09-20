#include <iostream>
#include "../include/Reservation.h"
#include "../include/ReservationManager.h"

int main()
{
    ReservationManager manager;

    // Create test reservations
    Reservation r1("RES1", "1001", "Alice Smith",
                   "R205", "2026-09-20");

    Reservation r2("RES2", "1002", "Bob Jones",
                   "R101", "2026-09-21");

    Reservation r3("RES3", "1003", "Charlie Brown",
                   "R310", "2026-09-22");

    // Add reservations to linked list
    manager.addReservation(r1);
    manager.addReservation(r2);
    manager.addReservation(r3);
    std::cout << "\nTesting duplicate RES1..." << std::endl;
    std::cout << "ACTIVE RESERVATIONS" << std::endl;
    std::cout << "===================" << std::endl;
    manager.displayReservations();

    // Test search
    std::cout << "\nSearching for RES2..." << std::endl;

    if (manager.searchReservation("RES2"))
        std::cout << "Reservation found." << std::endl;
    else
        std::cout << "Reservation not found." << std::endl;

    // Test edit
    std::cout << "\nChanging RES2 date..." << std::endl;

    if (manager.editReservation("RES2", "2026-09-30"))
        std::cout << "Reservation updated." << std::endl;
    else
        std::cout << "Reservation not found." << std::endl;

    // Test removal
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