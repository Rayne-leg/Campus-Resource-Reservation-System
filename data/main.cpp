#include <iostream>
#include <limits>

#include "ResourceManager.h"
#include "ReservationList.h"   
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
                    ReservationList& reservationList,
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
    reservationList.addReservation(newReservation);

    // IMPORTANT!!! rn ResourceManager has no way to flip a resource's
    // availability, so the "Available"/"Unavailable" column cant update
    // here. someone has to add something like
    // ResourceManager::setAvailability(resourceID, status) for that

    std::cout << "Reservation " << reservationID << " created for " << resourceID << "." << std::endl;
}

void handleEdit(ReservationList& reservationList)
{
    std::string reservationID, newDate;

    std::cout << "Enter Reservation ID to edit: ";
    std::cin >> reservationID;
    std::cout << "Enter new date: ";
    std::cin >> newDate;

    if (reservationList.editReservation(reservationID, newDate))
    {
        std::cout << "Reservation updated." << std::endl;
    }
    else
    {
        std::cout << "Reservation ID not found." << std::endl;
    }
}

void handleCancel(ResourceManager& resourceManager,
                   ReservationList& reservationList,
                   WaitingList& waitingList,
                   CancellationStack& cancellationStack)
{
    std::string reservationID;

    std::cout << "Enter Reservation ID to cancel: ";
    std::cin >> reservationID;

    Reservation removed;

    if (!reservationList.removeReservation(reservationID, removed))
    {
        std::cout << "Reservation ID not found." << std::endl;
        return;
    }

    // record the cancellation on the history stack
    std::string cancelDate = "today";
    cancellationStack.push(CancelledReservation(removed.reservationID,
                                                 removed.studentID,
                                                 removed.studentName,
                                                 removed.resourceID,
                                                 removed.reservationDate,
                                                 cancelDate));


    WaitingRequest nextInLine;
    bool promoted = false;


    WaitingList holding;
    while (waitingList.dequeue(nextInLine))
    {
        if (!promoted && nextInLine.resourceID == removed.resourceID)
        {
            std::string newResID = "RES" + std::to_string(nextReservationNumber++);
            Reservation promotedReservation(newResID,
                                             nextInLine.studentID,
                                             nextInLine.studentName,
                                             nextInLine.resourceID,
                                             nextInLine.reservationDate);
            reservationList.addReservation(promotedReservation);
            std::cout << "Promoted " << nextInLine.studentName
                      << " from the waiting list to reservation " << newResID << "." << std::endl;
            promoted = true;
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

void handleUndo(ReservationList& reservationList,
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

    reservationList.addReservation(restored);

    std::cout << "Restored reservation " << lastCancelled.reservationID << "." << std::endl;
}

int main()
{
    ResourceManager resourceManager;
    ReservationList reservationList;   
    WaitingList waitingList;          
    CancellationStack cancellationStack; 

    if (!resourceManager.loadResources("resources.txt"))
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
                handleReserve(resourceManager, reservationList, waitingList);
                break;
            case 2:
                reservationList.displayAll();
                break;
            case 3:
                handleEdit(reservationList);
                break;
            case 4:
                handleCancel(resourceManager, reservationList, waitingList, cancellationStack);
                break;
            case 5:
                handleUndo(reservationList, cancellationStack);
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
