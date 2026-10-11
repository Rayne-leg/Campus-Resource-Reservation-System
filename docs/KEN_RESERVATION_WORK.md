# Ken / Parbat Thapa — Reservation Component

## Implemented features

- Active reservations stored in a singly linked list.
- Reservation creation, display, search, editing, and removal.
- Duplicate reservation-ID and resource/date conflict checks.
- Manual linear search connected to menu option 10.
- Manual stable merge sort by reservation date, connected to option 11.
- Live active-reservation counts for each resource.
- Reservation-file loading from data/reservations.txt.
- Calendar-date validation and conversion to YYYY-MM-DD.
- Startup loading and active-reservation report in the menu.
- Generated RES IDs checked against existing active IDs.
- Date validation for reservation creation and editing.
- Failed undo returns the cancellation record to the history stack.
- Linked-list memory cleanup.

## Algorithms and complexity

Let n be the number of active reservations.

| Operation | Worst-case time | Extra space |
|---|---|---|
| Linear search by reservation ID | O(n) | O(1) |
| Count reservations for one resource | O(n) | O(1) |
| Display active reservations | O(n) | O(1) |
| Check resource/date conflict | O(n) | O(1) |
| Add reservation, including validation scans | O(n) | O(1) per new node |
| Remove reservation by ID | O(n) | O(1) |
| Edit reservation date | O(n) | O(1) |
| Merge sort by date | O(n log n) | O(log n) |
| Load n reservations into an empty list | O(n²) | O(n) stored nodes |
| Destroy the linked list | O(n) | O(1) |

Editing performs a search followed by a conflict scan. These scans
are sequential, so the overall time is O(n).

Merge sort splits the list using slow and fast pointers. It recursively
sorts each half and merges them by relinking existing nodes. The merge
is iterative. The recursive splitting requires O(log n) stack space.

Equal dates preserve their previous list order. Sorting compares
YYYY-MM-DD strings, which have the same order as calendar dates.
No library search or sorting function implements the required algorithms.

## Local testing results

Environment: MacBook, g++ command using the Apple clang++ toolchain.
Flags: -std=c++17 -Wall -Wextra -pedantic.
Date: October 11, 2026.

| Test | Result |
|---|---|
| Full application compilation | Passed without reported errors or warnings |
| Count: empty list and missing resource | Passed |
| Count: same resource on different dates | Passed |
| Count: removal and restoration | Passed |
| Sort: empty and one-node lists | Passed |
| Sort: dates across months and years | Passed |
| Sort: equal dates preserve order | Passed |
| Sort: repeated sorting preserves output | Passed |
| Sort: editing and removal after sorting | Passed |
| Load: all 20 supplied IDs found | Passed |
| Load: expected resource counts | Passed |
| Date conversion and leap-year checks | Passed |
| Rejection of invalid calendar dates | Passed |
| Menu option 11 displays all 20 dates in order | Passed |
| Menu option 10: existing and missing IDs | Pending documented manual verification |
| CELL component tests | Pending |
| Final combined system on CELL | Pending |

Test source files:
- tests/reservation_count_test.cpp
- tests/reservation_sort_test.cpp
- tests/reservation_load_test.cpp

## User instructions

Run from the project root so relative data paths resolve correctly.

The reservation file format is:
reservationID|studentID|studentName|resourceID|date

Dates accept MM/DD/YYYY or YYYY-MM-DD and are normalized when loaded.
Existing numeric-string IDs such as 301 are preserved.

- Option 2: display active reservations.
- Option 10: enter an exact reservation ID to check whether it exists.
- Option 11: sort and display reservations from earliest to latest date.
- Option 0: exit.

Adding or editing records can change their date order. Select option 11
again when a sorted display is needed.

Reservations are loaded at startup. Runtime edits are not saved back
to the file. The sample records are not automatically expired.

## Integration notes

Reports should call:
int countReservationsForResource(const std::string& resourceID) const;

This function reads the current linked list, so changes from addition,
cancellation, and restoration are reflected immediately.

ReservationManager::normalizeDate(input, output) provides shared date
validation. Dates must be normalized before resource/date comparisons.

Favour's reports and date-specific availability/waiting-list patch still
need integration. The current menu retains the earlier global availability
policy and resource-only waiting-list promotion.

Ademola should review the menu and undo changes rather than replacing
main.cpp without preserving startup loading and options 10 and 11.

The loader still needs resource-ID validation against ResourceManager.
Malformed-row, missing-file, and duplicate-load behavior need additional
documented tests.

Final integrated CELL testing remains required.

## Contribution report entry

| Team Member | Components | Specific Tasks Completed | Testing/Debugging | GitHub Contributions |
|---|---|---|---|---|
| Ken / Parbat Thapa | Reservations, active linked list, searching, sorting, reservation reporting support | Implemented resource counts, manual merge sort, file loading, date validation, startup loading, and search/sort menu integration; maintained existing reservation operations | Created and ran count, sort, and loading tests; corrected header placement and sorting function paste errors; verified sorted menu output | reservations branch: 20ad7d9, 0e1e899, 5c7d672, 9a69001, 4957e2f |

My part was the reservation manager and active-reservation linked list.
I added the counting function used to connect the resource reports,
implemented merge sort by date, and added loading for the sample
reservation file. I connected the existing linear search and new sort
to the menu and added date validation. I tested counts after removal
and restoration, ordering across different dates, equal-date ordering,
and loading the supplied data. I also updated the menu's error messages
and preserved cancellation history when undo fails. My changes are on
the reservations branch. Combined integration and CELL verification
are still pending.

## Commit references

- 20ad7d9: Add sample reservation data
- 0e1e899: Add resource reservation counts and tests
- 5c7d672: Implement reservation merge sort and tests
- 9a69001: Add reservation file loading and date validation tests
- 4957e2f: Integrate reservation loading search and sorting into menu

Repository:
https://github.com/Rayne-leg/Campus-Resource-Reservation-System

## Validation update — October 11, 2026

This section supersedes the earlier pending items for menu searching
and loader validation.

- Menu search for 301: passed.
- Menu search for 999: passed with not-found message.
- Resource-ID validation connected to ResourceManager during startup.
- Unknown resource, malformed row, invalid date, duplicate ID,
  and resource/date conflict rejection: passed.
- Valid rows remain loaded when other rows are rejected: passed.
- Repeated loading does not duplicate reservations: passed.
- Missing-file handling preserves existing reservations: passed.
- Direct add/edit calls reject invalid calendar dates: passed.
- Count, sort, and loading tests rebuilt and passed after these changes.
- Accidental copying of ReservationManager is disabled to prevent
  multiple managers owning the same linked-list nodes.

Resource-ID validation uses a callback. With r resources and n input
reservations, loading into an empty manager takes O(n² + nr) worst-case
time because resource lookup and reservation validation use linear scans.

CELL testing and final combined-system verification remain pending.
