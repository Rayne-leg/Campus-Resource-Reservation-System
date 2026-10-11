#include "ReservationManager.h"
#include <cassert>
#include <iostream>

int main()
{
    ReservationManager manager;
    std::string date;

    assert(ReservationManager::normalizeDate("09/15/2026", date));
    assert(date == "2026-09-15");

    assert(ReservationManager::normalizeDate("2026-09-15", date));
    assert(date == "2026-09-15");

    assert(ReservationManager::normalizeDate("02/29/2024", date));
    assert(!ReservationManager::normalizeDate("02/29/2026", date));
    assert(!ReservationManager::normalizeDate("2026-04-31", date));
    assert(!ReservationManager::normalizeDate("not-a-date", date));

    assert(manager.loadReservations("data/reservations.txt"));

    for (int id = 301; id <= 320; id++)
    {
        assert(manager.searchReservation(std::to_string(id)));
    }

    assert(manager.countReservationsForResource("R101") == 2);
    assert(manager.countReservationsForResource("R119") == 2);
    assert(manager.countReservationsForResource("R120") == 0);

    // Confirms the loaded date was converted to YYYY-MM-DD.
    assert(manager.hasResourceDateConflict("R101", "2026-09-15"));

    manager.sortByDate();

    Reservation removed;
    assert(manager.removeReservation("301", removed));
    assert(removed.getReservationDate() == "2026-09-15");
    assert(manager.countReservationsForResource("R101") == 1);

    std::cout << "All reservation loading tests passed.\n";
    return 0;
}
