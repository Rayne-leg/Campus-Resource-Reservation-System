# Cancellation stack and menu complexity

Let A be active reservations, R resources, W waiting requests, H cancellation-history records, and k the number of generated-ID attempts.

| Operation | Worst-case time | Extra space |
|---|---|---|
| Stack push, pop, peek, empty/size check | O(1) | O(1) per stored node |
| Display cancellation history | O(H) | O(1) |
| Destroy cancellation stack | O(H) | O(1) |
| Undo validation and restoration | O(R + A) | O(1) beyond the restored node |
| Cancellation with eligible queue promotion | O(R + W + (k + 1)A) | O(1) beyond stored reservation/history nodes |

Undo peeks at the top cancellation, checks resource existence, checks the reservation ID and resource/date conflict, and attempts insertion before popping. Sequential linear scans do not multiply their costs. A failed restore retains the top history record.

Cancellation first scans/removes the active reservation and pushes its details. It checks availability and scans for the earliest matching resource/date waiter, then inserts the promotion before dequeuing. Generated IDs are checked by linear search; k counts the ID candidates tried. Input parsing is linear in the entered line length. These bounds describe the combined implementation, including the resource and queue operations called by the menu.
