# Complexity notes for Favour's components

Let R be the number of resources, A the number of active reservations, and W the number of waiting requests. Ken's countReservationsForResource scans the one active linked list, so C(A) = O(A). Output length is proportional to the rows printed.

| Operation | Time | Extra space | Explanation |
|---|---|---|---|
| Resource utilization | O(R * C(A) + R) | O(1) | Calls the live count function once per resource and prints all rows. If Ken scans the linked list, this is O(RA + R). |
| Most requested resources | O(R * C(A) + R) | O(R) | Collects live counts once, finds the maximum by traversal, then prints every tie. It does not use a library searching/sorting routine. |
| Waiting requests for one resource | O(W) | O(1) | Scans queued requests and counts matching resource IDs. |
| Distinct waiting students for one resource | O(W squared) worst case | O(1) | Checks earlier queue nodes for the same resource/student pair, so repeated dates do not count the same student twice. |
| Waiting-list statistics | O(R * (W squared + W) + R) worst case | O(1) | Computes both counts for every resource and prints zero-count rows. |
| Date-specific availability check | O(R + A) | O(1) | Looks up the resource, then traverses the active-reservation list for the resource/date conflict. |
| Date-specific availability display | O(R * (R + A) + R) | O(1) | Runs the availability check for each resource. |
| Peek first matching waiting request | O(W) | O(1) | Traverses without mutating the queue. |
| Dequeue first matching resource/date | O(W) | O(1) | Traverses to the earliest match, repairs front/rear/next pointers, and deletes one node. |
| Check duplicate waiting request | O(W) | O(1) | Compares student ID, resource ID, and date while traversing the queue. |

The queue's ordinary enqueue/dequeue remain O(1). The matching operation is O(W) because unrelated requests may precede an eligible one. No additional active-reservation linked list is created.

These bounds describe the combined production components. Reservation algorithm complexity is in KEN_RESERVATION_WORK.md; stack and menu complexity is in ADEMOLA_COMPLEXITY.md.
