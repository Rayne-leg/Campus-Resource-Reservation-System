# Campus Resource Reservation System — Ken/Favour integration

CSCE 2110, Project 1 Final integration candidate. Based on the GitHub reservations branch ZIP supplied on October 11, 2026, with Favour's report and date/waiting-list changes. See docs/INTEGRATION_STATUS.md for verified checks and remaining team work.

## Build and run on CELL

From the project root:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude src/*.cpp -o campus_reservation
./campus_reservation
```

Run all five component test programs and the integrated menu scenarios:

```bash
bash tests/run_tests.sh
```

The runner requires g++ with C++17 and Python 3. It builds in a temporary directory and cleans its binaries. Success ends with ALL INTEGRATION CHECKS PASSED. Loader-error tests intentionally reject invalid data; their expected diagnostics are captured by the runner and shown if a test fails.

## Menu

| Option | Action |
|---|---|
| 1 | Reserve a resource or join its matching-date waiting list |
| 2 | Active reservations report |
| 3 | Edit a reservation date |
| 4 | Cancel and promote the first eligible resource/date waiter |
| 5 | Restore the most recent cancellation; preserve history if blocked |
| 6 | Display resource information and legacy file status |
| 7 | Check resource availability for a supplied date |
| 8 | Display waiting requests |
| 9 | Display cancellation history |
| 10 | Manual linear search by reservation ID |
| 11 | Manual stable merge sort by date, then display reservations |
| 12 | Resource utilization: all resources and current active counts |
| 13 | Most requested: resources tied for the largest active count |
| 14 | Waiting-list statistics: distinct students and requests per resource |
| 0 | Exit |

Dates accept YYYY-MM-DD or MM/DD/YYYY, are calendar-validated, and normalize to YYYY-MM-DD. Booking conflicts use the resource/date pair, not a global status flag. Availability information in resources.txt is legacy resource metadata, not the reservation calendar.

## Data and structures

- Resources: vector loaded from data/resources.txt.
- All active reservations: one owning linked list loaded from data/reservations.txt.
- Waiting requests: one FIFO queue carrying resource and date information; matching-date removal preserves order among remaining nodes.
- Cancellation history: one LIFO stack.

Reservation format: reservationID|studentID|studentName|resourceID|date. New IDs are generated as RES1, RES2, and so on, checking active IDs. Runtime changes are in memory and do not persist to input files. Sample reservations are not automatically expired.

## Team documentation

- docs/KEN_RESERVATION_WORK.md: Ken's submitted component notes and reported commits/testing.
- docs/FAVOUR_COMPLEXITY.md: Favour's actual component complexity.
- docs/INTEGRATION_STATUS.md: observed integration tests, pending CELL/team/GitHub work, and behavioral limits.
- docs/TEAM_UPDATE.txt: status text to adapt after the CELL run.
- tests/component_results.txt and tests/integration_menu_results.txt: observed local outputs.
- integration/changes_from_reservations.patch: source/documentation changes against the supplied reservations snapshot.

The instructor's final deliverables also require the actual GitHub URL, completed group contribution report, final testing, and demonstration. This candidate does not claim a final shared merge or completed submission.
