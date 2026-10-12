# Ken / Parbat Thapa — Reservation Work

## My contribution
- Updated ReservationManager and the active-reservation linked list.
- Connected manual linear search to menu option 10.
- Implemented manual merge sort by date for option 11.
- Added reservation counts for Favour's reports.
- Loaded data/reservations.txt and validated resource IDs.
- Added date validation and conversion to YYYY-MM-DD.
- Added component tests and updated menu handling.
- Preserved cancellation history when undo fails.

## Complexity
- Search, count, add, edit, remove, and display: O(n).
- Merge sort: O(n log n) time and O(log n) extra space.
- Loading n records with r resources: O(n² + nr) worst-case time.

## Testing
Local tests and CELL tests passed for counting, sorting, loading,
and loader error handling.

CELL server: cell16-cse
Date: October 11, 2026
Tested commit: cddb60d
Build flags: -std=c++17 -Wall -Wextra -pedantic

The application compiled without reported errors or warnings.
Menu search found 301 and rejected missing ID 999.
Sorting displayed all 20 reservations in chronological order.
The program exited normally.

## GitHub
Branch: reservations
Feature commits: 0e1e899, 5c7d672, 9a69001, 4957e2f
CELL-tested version, including later validation changes: cddb60d

