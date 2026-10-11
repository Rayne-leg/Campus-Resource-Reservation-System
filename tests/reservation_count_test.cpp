#include "ReservationManager.h"
#include <cassert>
#include <iostream>

int main()
{
    ReservationManager manager;

    assert(manager.countReservationsForResource("R101") == 0);

    assert(manager.addReservation(
        Reservation("RES1", "1001", "Alice", "R101", "2026-10-15")));

    assert(manager.addReservation(
        Reservation("RES2", "1002", "Bob", "R101", "2026-10-16")));

    assert(manager.addReservation(
        Reservation("RES3", "1003", "Sara", "R105", "2026-10-15")));

    assert(manager.countReservationsForResource("R101") == 2);
    assert(manager.countReservationsForResource("R105") == 1);
    assert(manager.countReservationsForResource("R999") == 0);

    Reservation removed;
    assert(manager.removeReservation("RES1", removed));
    assert(manager.countReservationsForResource("R101") == 1);

    assert(manager.addReservation(removed));
    assert(manager.countReservationsForResource("R101") == 2);

    std::cout << "All reservation count tests passed.\n";
    return 0;
}
