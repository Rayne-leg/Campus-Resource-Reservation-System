# Ken reservation component

Ken / Parbat Thapa implemented the one active-reservation linked list, reservation operations, manual linear search, stable manual merge sort, live resource counts, file loading, date normalization, and validation. The final combined menu uses option 15 for search, 13 for sorting, 2 for the active report, and 14 for file loading. Reservations load automatically at startup.

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

## Testing and GitHub contributions

Ken supplied count, sorting, loading, and loader-error tests. These are included in tests/run_tests.sh. Ken reported testing cddb60d on cell16-cse. Favour ran the Ken/Favour integration on CELL and shared its passing output. Favour also ran the updated combined menu and all five component programs on CELL; all checks passed, as recorded in tests/cell_results.txt and INTEGRATION_STATUS.md.

Branch: reservations. Reported commits:

- 20ad7d9: sample reservation data
- 0e1e899: counting method and tests
- 5c7d672: merge sort and tests
- 9a69001: reservation loading and date validation
- 4957e2f: search/sort menu integration
- cddb60d: resource validation and test documentation
- 045f77c: simplified documentation

With n reservations and r resources, loading into an empty manager is O(n squared + nr) in the worst case, including resource validation. Runtime edits are not saved back to the input file. The supplied sample reservations remain active until cancelled.
