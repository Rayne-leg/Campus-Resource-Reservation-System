#include "ReservationManager.h"
#include <cassert>
#include <cstdio>
#include <fstream>
#include <iostream>

int main()
{
    const char* filename = "tests/loader_errors.tmp";

    {
        std::ofstream file(filename);
        assert(file.is_open());

        file << "A|1001|Alice|R101|10/15/2026\n";
        file << "B|1002|Bob|R999|10/16/2026\n";
        file << "broken row\n";
        file << "C|1003|Sara|R101|02/29/2026\n";
        file << "A|1004|David|R101|10/17/2026\n";
        file << "D|1005|Emma|R101|10/15/2026\n";
        file << "E|1006|Noah|R101|10/18/2026\n";
    }

    ReservationManager manager;

    auto resourceExists = [](const std::string& resourceID)
    {
        return resourceID == "R101";
    };

    assert(!manager.loadReservations(filename, resourceExists));

    assert(manager.searchReservation("A"));
    assert(manager.searchReservation("E"));
    assert(!manager.searchReservation("B"));
    assert(!manager.searchReservation("C"));
    assert(!manager.searchReservation("D"));
    assert(manager.countReservationsForResource("R101") == 2);

    assert(!manager.loadReservations(filename, resourceExists));
    assert(manager.countReservationsForResource("R101") == 2);

    assert(std::remove(filename) == 0);

    assert(!manager.loadReservations(filename, resourceExists));
    assert(manager.countReservationsForResource("R101") == 2);

    assert(!manager.addReservation(
        Reservation("F", "1007", "Mia", "R101", "2026-04-31")));

    assert(!manager.editReservation("A", "2026-02-30"));
    assert(manager.hasResourceDateConflict("R101", "2026-10-15"));

    std::cout << "All reservation loader error tests passed.\n";
    return 0;
}
