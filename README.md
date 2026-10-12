# Campus Resource Reservation System

CSCE 2110 Project 1 Final. Team: Favour Oviosa, Ken / Parbat Thapa, and Ademola D.

The program stores all active reservations in one linked list, waiting requests in a FIFO queue, and cancellation history in a LIFO stack. It combines Ken's reservation algorithms and data, Favour's reports and date-specific waiting lists, and Ademola's updated menu, undo checks, and input handling.

## Build and run on UNT CELL

Run from the project root so that the relative data paths resolve correctly:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude src/*.cpp -o campus_reservation
./campus_reservation
```

Run the component and complete menu tests:

```bash
bash tests/run_tests.sh
```

The runner requires C++17 g++ and Python 3. It builds with `-Wall -Wextra -Werror -pedantic`, uses a temporary directory for binaries, and ends with `ALL INTEGRATION CHECKS PASSED` on success. Expected loader rejection diagnostics are captured by the component runner. The menu regression test stores its transcript in tests/integration_menu_results.txt.

## Menu and user instructions

| Option | Action |
|---|---|
| 1 | Reserve a resource; join its waiting list if that resource and date are occupied |
| 2 | Display the active reservation report |
| 3 | Edit an existing reservation date |
| 4 | Cancel a reservation and promote the earliest eligible resource/date waiter |
| 5 | Restore the most recent cancellation when its ID and resource/date are free |
| 6 | Display all resource information |
| 7 | Display availability for a supplied date |
| 8 | Display current waiting requests |
| 9 | Display cancellation history, newest first |
| 10 | Resource utilization report for every resource |
| 11 | Most requested resources, including ties |
| 12 | Waiting-list statistics for every resource |
| 13 | Sort active reservations by date and display them |
| 14 | Load additional reservations; blank filename uses data/reservations.txt |
| 15 | Search active reservations by exact reservation ID |
| 0 | Exit |

Start with option 6 to find resource IDs, and option 2 to see loaded reservation IDs. Option 1 asks for a resource ID, student ID, student name, and date. Dates accept YYYY-MM-DD or MM/DD/YYYY and normalize to YYYY-MM-DD. Invalid dates are rejected and prompted again; an empty date cancels the operation. Names and IDs have surrounding whitespace removed. Invalid menu input produces an error and returns to the menu; end of input exits cleanly.

For a quick demonstration, reserve R101 for 2026-10-11 and 2026-10-12 using different student IDs. Both bookings succeed. Another student requesting either occupied date joins its waiting list. Cancel the booking for one date: the earliest waiter for that same resource and date is promoted, while requests for the other date stay queued. A blocked undo keeps the cancellation on top of the stack. Options 10-12 show current counts after these changes. Option 15 searches for 301; option 13 shows dates in ascending order.

## Data and required algorithms

Resources are loaded from data/resources.txt into a vector. Reservations are loaded from data/reservations.txt at startup with resource-ID validation. The reservation file has one record per line:

```text
reservationID|studentID|studentName|resourceID|date
```

Invalid rows, duplicate IDs, invalid dates, unknown resources, and resource/date conflicts are reported and skipped; valid rows remain loaded. Repeated loading does not duplicate records. New RES IDs skip IDs already active.

ReservationManager implements manual linear search and stable linked-list merge sort. Both are integrated into the menu; no library searching or sorting routine implements the required algorithms. Additions and edits can change date order, so sort again when needed.

Resource utilization includes every resource and its live active-reservation count. Most requested uses the highest current active-reservation count and shows every tie. Waiting-list statistics shows distinct student IDs and queued requests per resource, including zero counts. A student waiting for multiple dates of the same resource counts once as a waiting student. These reports use the current linked list and queue, without hard-coded results.

## Behavior and documentation

Resource availability depends on the resource/date pair in the active linked list. The status stored in resources.txt is legacy metadata, not a global booking calendar. Waiting requests are matched by resource and date; matching removal preserves FIFO order. Cancelling stores the original reservation on the stack. Undo peeks and validates first, adds the reservation, and pops the cancellation only after restoration succeeds.

Runtime changes remain in memory and are not saved to the input files. Sample September reservations remain active until cancelled; there is no automatic expiry. Cancellation history currently displays the literal `today` as its cancellation label.

- Group_Contribution_Report.docx: team contribution matrix and verification record.
- docs/FAVOUR_COMPLEXITY.md: reports, availability, and queue complexity.
- docs/KEN_RESERVATION_WORK.md: reservation algorithms and complexity.
- docs/ADEMOLA_COMPLEXITY.md: stack and menu integration complexity.
- docs/INTEGRATION_STATUS.md: source provenance and actual testing evidence.
- tests/: component tests and menu regression checks.
- GitHub_Repository_URL.txt: the shared repository URL.

Combined source CELL verification: passed on cell16-cse on October 11, 2026. Favour ran bash tests/run_tests.sh in ~/Final_Team_Integration; all five component programs and the expanded menu scenarios passed. See tests/cell_results.txt and docs/INTEGRATION_STATUS.md. The C++ source and test programs are unchanged from that run.

Repository: https://github.com/Rayne-leg/Campus-Resource-Reservation-System
