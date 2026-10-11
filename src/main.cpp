#include <iostream>
#include <limits>
#include <string>

#include "ResourceManager.h"
#include "ReservationManager.h"
#include "WaitingList.h"
#include "CancellationStack.h"

int nextReservationNumber = 1;

void printMenu()
{
    std::cout << "\n===== CAMPUS RESOURCE RESERVATION SYSTEM =====\n";
    std::cout << "1. Reserve a resource\n";
    std::cout << "2. View active reservations\n";
    std::cout << "3. Edit a reservation\n";
    std::cout << "4. Cancel a reservation\n";
    std::cout << "5. Undo last cancellation\n";
    std::cout << "6. Display all resources\n";
    std::cout << "7. Display resource availability\n";
    std::cout << "8. Display waiting list\n";
    std::cout << "9. Display cancellation history\n";
    std::cout << "10. Search reservation by ID\n";
    std::cout << "11. Sort active reservations by date\n";
    std::cout << "0. Exit\n";
    std::cout << "Choose an option: ";
}

void clearInputError()
{
    std::cin.clear();
    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(), '\n');
}

bool readDate(std::string& date)
{
    std::string input;
    std::cout << "Enter date (YYYY-MM-DD or MM/DD/YYYY): ";

    if (!(std::cin >> input))
    {
        return false;
    }

    if (!ReservationManager::normalizeDate(input, date))
    {
        std::cout << "Invalid calendar date.\n";
        return false;
    }

    return true;
}

std::string generateReservationID(
    const ReservationManager& reservationManager)
{
    std::string id;

    do
    {
        id = "RES" + std::to_string(nextReservationNumber++);
    }
    while (reservationManager.searchReservation(id));

    return id;
}

void handleReserve(
    ResourceManager& resourceManager,
    ReservationManager& reservationManager,
    WaitingList& waitingList)
{
    std::string resourceID, studentID, studentName, date;

    std::cout << "Enter Resource ID: ";

    if (!(std::cin >> resourceID))
    {
        return;
    }

    if (!resourceManager.resourceExists(resourceID))
    {
        std::cout << "That resource does not exist.\n";
        return;
    }

    std::cout << "Enter Student ID: ";

    if (!(std::cin >> studentID))
    {
        return;
    }

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Enter Student Name: ";

    if (!std::getline(std::cin, studentName))
    {
        return;
    }

    if (studentName.find_first_not_of(" \t\r") ==
        std::string::npos)
    {
        std::cout << "Student name cannot be empty.\n";
        return;
    }

    if (!readDate(date))
    {
        return;
    }

    // Replace this policy with Favour's date-specific integration.
    if (!resourceManager.isResourceAvailable(resourceID) ||
        reservationManager.hasResourceDateConflict(resourceID, date))
    {
        waitingList.enqueue(
            WaitingRequest(studentID, studentName, resourceID, date));

        std::cout << "Resource unavailable. Added to waiting list.\n";
        return;
    }

    std::string id = generateReservationID(reservationManager);
    Reservation reservation(id, studentID, studentName, resourceID, date);

    if (!reservationManager.addReservation(reservation))
    {
        std::cout << "Reservation rejected.\n";
        return;
    }

    resourceManager.setAvailability(resourceID, "Unavailable");
    std::cout << "Reservation " << id << " created.\n";
}

void handleEdit(ReservationManager& reservationManager)
{
    std::string id, date;
    std::cout << "Enter Reservation ID to edit: ";

    if (!(std::cin >> id))
    {
        return;
    }

    if (!reservationManager.searchReservation(id))
    {
        std::cout << "Reservation not found.\n";
        return;
    }

    if (!readDate(date))
    {
        return;
    }

    if (reservationManager.editReservation(id, date))
    {
        std::cout << "Reservation updated.\n";
    }
    else
    {
        std::cout << "That resource is already reserved on that date.\n";
    }
}

void handleSearch(const ReservationManager& reservationManager)
{
    std::string id;
    std::cout << "Enter Reservation ID to search: ";

    if (!(std::cin >> id))
    {
        return;
    }

    if (reservationManager.searchReservation(id))
    {
        std::cout << "Reservation " << id << " found.\n";
    }
    else
    {
        std::cout << "Reservation not found.\n";
    }
}

void handleCancel(
    ResourceManager& resourceManager,
    ReservationManager& reservationManager,
    WaitingList& waitingList,
    CancellationStack& cancellationStack)
{
    std::string id;
    std::cout << "Enter Reservation ID to cancel: ";

    if (!(std::cin >> id))
    {
        return;
    }

    Reservation removed;

    if (!reservationManager.removeReservation(id, removed))
    {
        std::cout << "Reservation not found.\n";
        return;
    }

    cancellationStack.push(CancelledReservation(
        removed.getReservationID(),
        removed.getStudentID(),
        removed.getStudentName(),
        removed.getResourceID(),
        removed.getReservationDate(),
        "today"));

    WaitingRequest request;
    WaitingList holding;
    bool promoted = false;

    // Replace with Favour's resource/date promotion integration.
    while (waitingList.dequeue(request))
    {
        if (!promoted &&
            request.resourceID == removed.getResourceID())
        {
            std::string newID = generateReservationID(reservationManager);

            Reservation replacement(
                newID, request.studentID, request.studentName,
                request.resourceID, request.reservationDate);

            if (reservationManager.addReservation(replacement))
            {
                std::cout << "Promoted " << request.studentName
                          << " to reservation " << newID << ".\n";
                promoted = true;
            }
            else
            {
                holding.enqueue(request);
            }
        }
        else
        {
            holding.enqueue(request);
        }
    }

    while (holding.dequeue(request))
    {
        waitingList.enqueue(request);
    }

    if (!promoted &&
        reservationManager.countReservationsForResource(
            removed.getResourceID()) == 0)
    {
        resourceManager.setAvailability(
            removed.getResourceID(), "Available");
    }

    std::cout << "Reservation " << id << " cancelled.\n";
}

void handleUndo(
    ResourceManager& resourceManager,
    ReservationManager& reservationManager,
    CancellationStack& cancellationStack)
{
    CancelledReservation cancelled;

    if (!cancellationStack.pop(cancelled))
    {
        return;
    }

    Reservation restored(
        cancelled.reservationID,
        cancelled.studentID,
        cancelled.studentName,
        cancelled.resourceID,
        cancelled.reservationDate);

    if (!reservationManager.addReservation(restored))
    {
        cancellationStack.push(cancelled);
        std::cout << "Cannot restore: duplicate ID or date conflict.\n";
        return;
    }

    resourceManager.setAvailability(
        cancelled.resourceID, "Unavailable");

    std::cout << "Restored reservation "
              << cancelled.reservationID << ".\n";
}

int main()
{
    ResourceManager resourceManager;
    ReservationManager reservationManager;
    WaitingList waitingList;
    CancellationStack cancellationStack;

    if (!resourceManager.loadResources("data/resources.txt"))
    {
        std::cout << "Cannot start without the resource file.\n";
        return 1;
    }

    if (!reservationManager.loadReservations(
            "data/reservations.txt",
            [&resourceManager](const std::string& resourceID)
            {
                return resourceManager.resourceExists(resourceID);
            }))
    {
        std::cout
            << "Warning: reservation file missing or some rows rejected.\n";
    }

    int choice = -1;

    while (true)
    {
        printMenu();

        if (!(std::cin >> choice))
        {
            if (std::cin.eof())
            {
                break;
            }

            clearInputError();
            std::cout << "Please enter a number.\n";
            continue;
        }

        if (choice == 0)
        {
            std::cout << "Goodbye.\n";
            break;
        }

        switch (choice)
        {
            case 1:
                handleReserve(
                    resourceManager, reservationManager, waitingList);
                break;

            case 2:
                std::cout << "\n===== ACTIVE RESERVATIONS =====\n";
                reservationManager.displayReservations();
                break;

            case 3:
                handleEdit(reservationManager);
                break;

            case 4:
                handleCancel(resourceManager, reservationManager,
                             waitingList, cancellationStack);
                break;

            case 5:
                handleUndo(resourceManager, reservationManager,
                           cancellationStack);
                break;

            case 6:
                resourceManager.displayAllResources();
                break;

            case 7:
                resourceManager.displayAvailability();
                break;

            case 8:
                waitingList.displayWaitingList();
                break;

            case 9:
                cancellationStack.displayHistory();
                break;

            case 10:
                handleSearch(reservationManager);
                break;

            case 11:
                reservationManager.sortByDate();
                std::cout << "\n===== RESERVATIONS SORTED BY DATE =====\n";
                reservationManager.displayReservations();
                break;

            default:
                std::cout << "Invalid option.\n";
        }
    }

    return 0;
}