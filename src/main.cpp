#include <iostream>
#include <sstream>
#include <string>

#include "ResourceManager.h"
#include "ReservationManager.h"
#include "WaitingList.h"
#include "CancellationStack.h"
#include "ResourceReports.h"

int nextReservationNumber = 1;

bool readLine(const std::string& prompt, std::string& out)
{
    std::cout << prompt;
    if (!std::getline(std::cin, out))
    {
        return false;
    }

    const std::string whitespace = " \t\r\n";
    const std::size_t first = out.find_first_not_of(whitespace);
    if (first == std::string::npos)
    {
        out.clear();
        return true;
    }
    const std::size_t last = out.find_last_not_of(whitespace);
    out = out.substr(first, last - first + 1);
    return true;
}

// prompts until a valid date is entered, gives false on EOF.
// entering nothing cancels everyhing (returns false with date empty).
// uses ReservationManager::normalizeDate so there is one set of date rules:
// YYYY-MM-DD or MM/DD/YYYY are accepted, and the date comes back as YYYY-MM-DD.
bool readDate(const std::string& prompt, std::string& date)
{
    while (true)
    {
        std::string entered;
        if (!readLine(prompt, entered))
        {
            return false;
        }
        if (entered.empty())
        {
            std::cout << "No date entered; operation cancelled." << std::endl;
            return false;
        }
        if (ReservationManager::normalizeDate(entered, date))
        {
            return true;
        }
        std::cout << "Invalid date. Use YYYY-MM-DD (e.g. 2026-09-25) or MM/DD/YYYY, "
                  << "or press Enter to cancel." << std::endl;
    }
}

// next auto-generated reservation id that is not already in use. Needed because
// reservations loaded from a file may already use RES1, RES2, ...
std::string nextFreeReservationID(const ReservationManager& reservationManager)
{
    std::string id;
    do
    {
        id = "RES" + std::to_string(nextReservationNumber++);
    } while (reservationManager.searchReservation(id));
    return id;
}

// ---------------------------------------------------------------------------
// menu
// ---------------------------------------------------------------------------

void printMenu()
{
    std::cout << "\n===== COLLEGE RESOURCE RESERVATION SYSTEM =====" << std::endl;
    std::cout << "1. Reserve a resource" << std::endl;
    std::cout << "2. View all reservations" << std::endl;
    std::cout << "3. Edit a reservation" << std::endl;
    std::cout << "4. Cancel a reservation" << std::endl;
    std::cout << "5. Undo last cancellation" << std::endl;
    std::cout << "6. Display all resources" << std::endl;
    std::cout << "7. Display resource availability for a date" << std::endl;
    std::cout << "8. Display waiting list" << std::endl;
    std::cout << "9. Display cancellation history" << std::endl;
    std::cout << "10. Resource utilization report" << std::endl;
    std::cout << "11. Most requested resources report" << std::endl;
    std::cout << "12. Waiting-list statistics report" << std::endl;
    std::cout << "13. Sort reservations by date" << std::endl;
    std::cout << "14. Load reservations from file" << std::endl;
    std::cout << "15. Search for a reservation by ID" << std::endl;
    std::cout << "0. Exit" << std::endl;
}

// ---------------------------------------------------------------------------
// handlers
// ---------------------------------------------------------------------------

void handleReserve(ResourceManager& resourceManager,
                   ReservationManager& reservationManager,
                   WaitingList& waitingList)
{
    std::string resourceID, studentID, studentName, date;

    if (!readLine("Enter Resource ID: ", resourceID))
        return;

    if (!resourceManager.resourceExists(resourceID))
    {
        std::cout << "That resource does not exist." << std::endl;
        return;
    }

    if (!readLine("Enter Student ID: ", studentID) || studentID.empty())
    {
        std::cout << "Student ID is required." << std::endl;
        return;
    }
    if (!readLine("Enter Student Name: ", studentName) || studentName.empty())
    {
        std::cout << "Student name is required." << std::endl;
        return;
    }
    if (!readDate("Enter Reservation Date (YYYY-MM-DD or MM/DD/YYYY): ", date))
        return;

    // availability is per resource and date, a booking on one date cant
    // block the same resource on a diff date
    if (!resourceManager.isResourceAvailableOnDate(resourceID, date, reservationManager))
    {
        if (waitingList.hasWaitingRequest(studentID, resourceID, date))
        {
            std::cout << "You are already waiting for that resource and date." << std::endl;
            return;
        }
        std::cout << resourceID << " is reserved for " << date
                  << ". Adding you to the waiting list." << std::endl;
        waitingList.enqueue(WaitingRequest(studentID, studentName, resourceID, date));
        return;
    }

    std::string reservationID = nextFreeReservationID(reservationManager);

    Reservation newReservation(reservationID, studentID, studentName, resourceID, date);

    if (!reservationManager.addReservation(newReservation))
    {
        std::cout << "Could not create reservation (duplicate ID or date conflict)." << std::endl;
        return;
    }

    std::cout << "Reservation " << reservationID << " created for " << resourceID
              << " on " << date << "." << std::endl;
}

void handleEdit(ReservationManager& reservationManager)
{
    std::string reservationID, newDate;

    if (!readLine("Enter Reservation ID to edit: ", reservationID))
        return;

    // check existence first so not found and date conflict are reported
    // as the different problems they are
    if (!reservationManager.searchReservation(reservationID))
    {
        std::cout << "Reservation ID not found." << std::endl;
        return;
    }

    if (!readDate("Enter new date (YYYY-MM-DD or MM/DD/YYYY): ", newDate))
        return;

    if (reservationManager.editReservation(reservationID, newDate))
    {
        std::cout << "Reservation updated." << std::endl;
    }
    else
    {
        std::cout << "Could not update: that resource is already reserved on "
                  << newDate << "." << std::endl;
    }
}

void handleCancel(ResourceManager& resourceManager,
                  ReservationManager& reservationManager,
                  WaitingList& waitingList,
                  CancellationStack& cancellationStack)
{
    std::string reservationID;

    if (!readLine("Enter Reservation ID to cancel: ", reservationID))
        return;

    Reservation removed;

    if (!reservationManager.removeReservation(reservationID, removed))
    {
        std::cout << "Reservation ID not found." << std::endl;
        return;
    }

    // add the cancellation to the history stack
    const std::string cancelDate = "today";
    cancellationStack.push(CancelledReservation(removed.getReservationID(),
                                                removed.getStudentID(),
                                                removed.getStudentName(),
                                                removed.getResourceID(),
                                                removed.getReservationDate(),
                                                cancelDate));

    const std::string freedResource = removed.getResourceID();
    const std::string freedDate = removed.getReservationDate();

    // promote the earliest waiting request for specifically this resource and date only
    // peek first, so a failed add wont removes a student from the queue
    WaitingRequest nextInLine;
    if (resourceManager.isResourceAvailableOnDate(freedResource, freedDate,
                                                  reservationManager) &&
        waitingList.peekForResourceDate(freedResource, freedDate, nextInLine))
    {
        std::string newResID = nextFreeReservationID(reservationManager);
        Reservation promotedReservation(newResID,
                                        nextInLine.studentID,
                                        nextInLine.studentName,
                                        nextInLine.resourceID,
                                        nextInLine.reservationDate);

        if (reservationManager.addReservation(promotedReservation))
        {
            WaitingRequest removedRequest;
            waitingList.dequeueForResourceDate(freedResource, freedDate, removedRequest);
            std::cout << "Promoted " << nextInLine.studentName
                      << " from the waiting list to reservation " << newResID << "."
                      << std::endl;
        }
        else
        {
            std::cout << "Could not promote the waiting student; request remains queued."
                      << std::endl;
        }
    }

    std::cout << "Reservation " << reservationID << " cancelled." << std::endl;
}

// the undo :
//   * the history record is only removed after the reservation is frfr back
//   * if the original id is being used, or someone (like a promoted student) now
//     holds that resource/date, the restore is refused, the record stays on
//     top of the stack, and the message says which problem happened
void handleUndo(ResourceManager& resourceManager,
                ReservationManager& reservationManager,
                CancellationStack& cancellationStack)
{
    CancelledReservation lastCancelled;

    if (!cancellationStack.peek(lastCancelled))
    {
        std::cout << "Cancellation history is empty." << std::endl;
        return;
    }

    if (!resourceManager.resourceExists(lastCancelled.resourceID))
    {
        std::cout << "Could not restore " << lastCancelled.reservationID
                  << ": resource " << lastCancelled.resourceID
                  << " no longer exists. The history record was kept." << std::endl;
        return;
    }

    if (reservationManager.searchReservation(lastCancelled.reservationID))
    {
        std::cout << "Could not restore " << lastCancelled.reservationID
                  << ": that reservation ID is already in use. "
                  << "The history record was kept." << std::endl;
        return;
    }

    if (reservationManager.hasResourceDateConflict(lastCancelled.resourceID,
                                                   lastCancelled.reservationDate))
    {
        std::cout << "Could not restore " << lastCancelled.reservationID << ": "
                  << lastCancelled.resourceID << " is now reserved on "
                  << lastCancelled.reservationDate
                  << " (for example, a waiting student was promoted). "
                  << "The history record was kept." << std::endl;
        return;
    }

    Reservation restored(lastCancelled.reservationID,
                         lastCancelled.studentID,
                         lastCancelled.studentName,
                         lastCancelled.resourceID,
                         lastCancelled.reservationDate);

    if (!reservationManager.addReservation(restored))
    {
        std::cout << "Could not restore " << lastCancelled.reservationID
                  << ". The history record was kept." << std::endl;
        return;
    }

    // restore successful, only now can u remove the record from the stack
    CancelledReservation discarded;
    cancellationStack.pop(discarded);

    std::cout << "Restored reservation " << lastCancelled.reservationID
              << " for " << lastCancelled.resourceID
              << " on " << lastCancelled.reservationDate << "." << std::endl;
}

void handleSearch(const ReservationManager& reservationManager)
{
    std::string reservationID;

    if (!readLine("Enter Reservation ID to search for: ", reservationID))
        return;

    if (reservationManager.searchReservation(reservationID))
    {
        std::cout << "Reservation " << reservationID << " is an active reservation."
                  << std::endl;
    }
    else
    {
        std::cout << "Reservation ID not found." << std::endl;
    }
}

void handleLoad(const ResourceManager& resourceManager,
                ReservationManager& reservationManager)
{
    std::string filename;

    if (!readLine("Enter reservations file (blank for data/reservations.txt): ", filename))
        return;
    if (filename.empty())
    {
        filename = "data/reservations.txt";
    }

    // lines that fail validation are reported by loadReservations and skipped;
    // valid lines are still loaded.
    const bool allValid = reservationManager.loadReservations(
        filename,
        [&resourceManager](const std::string& id) { return resourceManager.resourceExists(id); });

    if (allValid)
    {
        std::cout << "Reservations loaded from " << filename << "." << std::endl;
    }
    else
    {
        std::cout << "Finished loading " << filename
                  << ", but some lines were skipped (see messages above)." << std::endl;
    }
}

// ---------------------------------------------------------------------------
// main
// ---------------------------------------------------------------------------

int main()
{
    ResourceManager resourceManager;
    ReservationManager reservationManager;
    WaitingList waitingList;
    CancellationStack cancellationStack;

    if (!resourceManager.loadResources("data/resources.txt"))
    {
        std::cout << "Warning: starting with no resources loaded." << std::endl;
    }

    // report adapter. declared after reservationManager so it never outlives it.
    // connected to ReservationManager::countReservationsForResource.
    ReservationCounter countForReports =
        [&reservationManager](const std::string& resourceID) -> int
        {
            return reservationManager.countReservationsForResource(resourceID);
        };

    int choice = -1;

    while (choice != 0)
    {
        printMenu();

        std::string line;
        if (!readLine("Choose an option: ", line))
        {
            std::cout << "\nInput ended. Goodbye." << std::endl;
            break;
        }

        //  only accetps a whole number; anything else is reported, not looped on
        std::istringstream parser(line);
        char extra;
        if (!(parser >> choice) || (parser >> extra))
        {
            choice = -1;
            std::cout << "Please enter a number." << std::endl;
            continue;
        }

        switch (choice)
        {
            case 1:
                handleReserve(resourceManager, reservationManager, waitingList);
                break;
            case 2:
                reservationManager.displayReservations();
                break;
            case 3:
                handleEdit(reservationManager);
                break;
            case 4:
                handleCancel(resourceManager, reservationManager, waitingList,
                             cancellationStack);
                break;
            case 5:
                handleUndo(resourceManager, reservationManager, cancellationStack);
                break;
            case 6:
                resourceManager.displayAllResources();
                break;
            case 7:
            {
                std::string date;
                if (readDate("Enter date to check (YYYY-MM-DD or MM/DD/YYYY): ", date))
                {
                    resourceManager.displayAvailabilityForDate(date, reservationManager);
                }
                break;
            }
            case 8:
                waitingList.displayWaitingList();
                break;
            case 9:
                cancellationStack.displayHistory();
                break;
            case 10:
                ResourceReports::displayResourceUtilization(resourceManager, countForReports);
                break;
            case 11:
                ResourceReports::displayMostRequestedResources(resourceManager, countForReports);
                break;
            case 12:
                ResourceReports::displayWaitingListStatistics(resourceManager, waitingList);
                break;
            case 13:
                reservationManager.sortByDate();
                std::cout << "Reservations sorted by date (earliest first)." << std::endl;
                reservationManager.displayReservations();
                break;
            case 14:
                handleLoad(resourceManager, reservationManager);
                break;
            case 15:
                handleSearch(reservationManager);
                break;
            case 0:
                std::cout << "Goodbye." << std::endl;
                break;
            default:
                std::cout << "Invalid option." << std::endl;
        }
    }

    return 0;
}
