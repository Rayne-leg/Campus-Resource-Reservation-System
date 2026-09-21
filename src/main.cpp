#include <iostream>
#include <limits>

#include "ResourceManager.h"
#include "ReservationManager.h"   
#include "WaitingList.h"       
#include "CancellationStack.h" 


int nextReservationNumber = 1;

void printMenu()
{
    std::cout << "\n===== COLLEGE RESOURCE RESERVATION SYSTEM =====" << std::endl;
    std::cout << "1. Reserve a resource" << std::endl;
    std::cout << "2. View all reservations" << std::endl;
    std::cout << "3. Edit a reservation" << std::endl;
    std::cout << "4. Cancel a reservation" << std::endl;
    std::cout << "5. Undo last cancellation" << std::endl;
    std::cout << "6. Display all resources" << std::endl;
    std::cout << "7. Display resource availability" << std::endl;
    std::cout << "8. Display waiting list" << std::endl;
    std::cout << "9. Display cancellation history" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "Choose an option: ";
}

void clearInputError()
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void handleReserve(ResourceManager& resourceManager,
                    ReservationManager& reservationManager,
                    WaitingList& waitingList)
{
    std::string resourceID, studentID, studentName, date;

    std::cout << "Enter Resource ID: ";
    std::cin >> resourceID;

    if (!resourceManager.resourceExists(resourceID))
    {
        std::cout << "That resource does not exist." << std::endl;
        return;
    }

    std::cout << "Enter Student ID: ";
    std::cin >> studentID;
    std::cout << "Enter Student Name: ";
    std::cin.ignore();
    std::getline(std::cin, studentName);
    std::cout << "Enter Reservation Date (e.g. 2026-09-25): ";
    std::cin >> date;

    if (!resourceManager.isResourceAvailable(resourceID))
    {
        // resource is taken, offers the waiting list instead
        std::cout << resourceID << " is currently unavailable. Adding you to the waiting list." << std::endl;
        waitingList.enqueue(WaitingRequest(studentID, studentName, resourceID, date));
        return;
    }

    std::string reservationID = "RES" + std::to_string(nextReservationNumber++);

    Reservation newReservation(reservationID, studentID, studentName, resourceID, date);

    if (!reservationManager.addReservation(newReservation))
    {
        // Shouldn't normally happen since IDs are auto-generated, but
        // ReservationManager now protects against duplicate IDs, so
        // handle a rejected add instead of assuming it always succeeds.
        std::cout << "Could not create reservation (duplicate ID)." << std::endl;
        return;
    }

    // IMPORTANT!!! rn ResourceManager has no way to flip a resource's
    // availability, so the "Available"/"Unavailable" column cant update
    // here. ken has to add something like
    // ResourceManager::setAvailability(resourceID, status) for that

    std::cout << "Reservation " << reservationID << " created for " << resourceID << "." << std::endl;
}

void handleEdit(ReservationManager& reservationManager)
{
    std::string reservationID, newDate;

    std::cout << "Enter Reservation ID to edit: ";
    std::cin >> reservationID;
    std::cout << "Enter new date: ";
    std::cin >> newDate;

    if (reservationManager.editReservation(reservationID, newDate))
    {
        std::cout << "Reservation updated." << std::endl;
    }
    else
    {
        std::cout << "Reservation ID not found." << std::endl;
    }
}

void handleCancel(ResourceManager& resourceManager,
                   ReservationManager& reservationManager,
                   WaitingList& waitingList,
                   CancellationStack& cancellationStack)
{
    std::string reservationID;

    std::cout << "Enter Reservation ID to cancel: ";
    std::cin >> reservationID;

    Reservation removed;

    if (!reservationManager.removeReservation(reservationID, removed))
    {
        std::cout << "Reservation ID not found." << std::endl;
        return;
    }

    // record the cancellation on the history stack
    std::string cancelDate = "today";
    cancellationStack.push(CancelledReservation(removed.getReservationID(),
                                                 removed.getStudentID(),
                                                 removed.getStudentName(),
                                                 removed.getResourceID(),
                                                 removed.getReservationDate(),
                                                 cancelDate));


    WaitingRequest nextInLine;
    bool promoted = false;


    WaitingList holding;
    while (waitingList.dequeue(nextInLine))
    {
        if (!promoted && nextInLine.resourceID == removed.getResourceID())
        {
            std::string newResID = "RES" + std::to_string(nextReservationNumber++);
            Reservation promotedReservation(newResID,
                                             nextInLine.studentID,
                                             nextInLine.studentName,
                                             nextInLine.resourceID,
                                             nextInLine.reservationDate);
            if (reservationManager.addReservation(promotedReservation))
            {
                std::cout << "Promoted " << nextInLine.studentName
                          << " from the waiting list to reservation " << newResID << "." << std::endl;
                promoted = true;
            }
            else
            {
                // Extremely unlikely (fresh generated ID), but don't
                // silently drop the student if it ever happens.
                holding.enqueue(nextInLine);
            }
        }
        else
        {
            holding.enqueue(nextInLine);
        }
    }
    while (holding.dequeue(nextInLine))
    {
        waitingList.enqueue(nextInLine);
    }



    std::cout << "Reservation " << reservationID << " cancelled." << std::endl;
}

void handleUndo(ReservationManager& reservationManager,
                 CancellationStack& cancellationStack)
{
    CancelledReservation lastCancelled;

    if (!cancellationStack.pop(lastCancelled))
    {
        return; // pop() already prints a message when empty
    }

    Reservation restored(lastCancelled.reservationID,
                          lastCancelled.studentID,
                          lastCancelled.studentName,
                          lastCancelled.resourceID,
                          lastCancelled.reservationDate);

    if (!reservationManager.addReservation(restored))
    {
        // Someone else grabbed the same ID while it sat on the stack
        // (shouldn't happen in normal use, but don't lose the record).
        std::cout << "Could not restore reservation " << lastCancelled.reservationID
                   << " (ID now in use)." << std::endl;
        return;
    }

    std::cout << "Restored reservation " << lastCancelled.reservationID << "." << std::endl;
}

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

    int choice = -1;

    while (choice != 0)
    {
        printMenu();

        if (!(std::cin >> choice))
        {
            clearInputError();
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
                handleCancel(resourceManager, reservationManager, waitingList, cancellationStack);
                break;
            case 5:
                handleUndo(reservationManager, cancellationStack);
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
            case 0:
                std::cout << "Goodbye." << std::endl;
                break;
            default:
                std::cout << "Invalid option." << std::endl;
        }
    }

    return 0;
}
