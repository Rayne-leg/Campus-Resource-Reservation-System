#include "ReservationManager.h"
#include <cassert>
#include <iostream>
#include <sstream>
#include <string>

std::string captureDisplay(const ReservationManager& manager)
{
    std::ostringstream output;
    std::streambuf* original = std::cout.rdbuf(output.rdbuf());
    manager.displayReservations();
    std::cout.rdbuf(original);
    return output.str();
}

int main()
{
    ReservationManager manager;

    // Empty list.
    manager.sortByDate();
    assert(!manager.searchReservation("missing"));

    // One reservation.
    assert(manager.addReservation(
        Reservation("RES1", "1001", "Alice", "R101", "2027-01-01")));

    manager.sortByDate();
    assert(manager.searchReservation("RES1"));

    // Add dates out of order, including equal dates.
    assert(manager.addReservation(
        Reservation("RES2", "1002", "Bob", "R102", "2026-12-31")));

    assert(manager.addReservation(
        Reservation("RES3", "1003", "Sara", "R103", "2026-02-01")));

    assert(manager.addReservation(
        Reservation("RES4", "1004", "David", "R104", "2026-02-01")));

    manager.sortByDate();

    std::string output = captureDisplay(manager);

    // Expected order: RES4, RES3, RES2, RES1.
    // RES4 was ahead of RES3 before sorting, so ties keep that order.
    std::size_t p4 = output.find("Reservation ID: RES4\n");
    std::size_t p3 = output.find("Reservation ID: RES3\n");
    std::size_t p2 = output.find("Reservation ID: RES2\n");
    std::size_t p1 = output.find("Reservation ID: RES1\n");

    assert(p4 != std::string::npos);
    assert(p3 != std::string::npos);
    assert(p2 != std::string::npos);
    assert(p1 != std::string::npos);
    assert(p4 < p3 && p3 < p2 && p2 < p1);

    assert(manager.countReservationsForResource("R101") == 1);
    assert(manager.countReservationsForResource("R104") == 1);

    // Sorting an already sorted list preserves the result.
    manager.sortByDate();
    assert(captureDisplay(manager) == output);

    // Editing and cancellation still work after sorting.
    assert(manager.editReservation("RES1", "2026-01-01"));
    manager.sortByDate();
    output = captureDisplay(manager);
    assert(output.find("Reservation ID: RES1\n") == 0);

    Reservation removed;
    assert(manager.removeReservation("RES3", removed));
    assert(!manager.searchReservation("RES3"));

    assert(manager.removeReservation("RES1", removed));
    assert(manager.removeReservation("RES2", removed));
    assert(manager.removeReservation("RES4", removed));

    manager.sortByDate();
    assert(captureDisplay(manager) == "No active reservations.\n");

    std::cout << "All reservation sort tests passed.\n";
    return 0;
}
