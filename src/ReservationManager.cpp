#include "ReservationManager.h"
#include <iostream>

ReservationManager::ReservationManager()
{
    head = nullptr;
}

ReservationManager::~ReservationManager()
{
    Node* current = head;

    while (current != nullptr)
    {
        Node* temp = current;
        current = current->next;
        delete temp;
    }
}

bool ReservationManager::addReservation(
    const Reservation& reservation)
{
    if (searchReservation(reservation.getReservationID()))
    {
        return false;
    }

    if (hasResourceDateConflict(
            reservation.getResourceID(),
            reservation.getReservationDate()))
    {
        return false;
    }

    Node* newNode = new Node;
    newNode->data = reservation;
    newNode->next = head;
    head = newNode;

    return true;
}

void ReservationManager::displayReservations() const
{
    if (head == nullptr)
    {
        std::cout << "No active reservations.\n";
        return;
    }

    Node* current = head;

    while (current != nullptr)
    {
        std::cout << "Reservation ID: "
                  << current->data.getReservationID() << '\n';

        std::cout << "Student ID: "
                  << current->data.getStudentID() << '\n';

        std::cout << "Student Name: "
                  << current->data.getStudentName() << '\n';

        std::cout << "Resource ID: "
                  << current->data.getResourceID() << '\n';

        std::cout << "Reservation Date: "
                  << current->data.getReservationDate() << '\n';

        std::cout << "------------------------\n";

        current = current->next;
    }
}

// Manual linear search: O(n).
bool ReservationManager::searchReservation(
    const std::string& reservationID) const
{
    Node* current = head;

    while (current != nullptr)
    {
        if (current->data.getReservationID() == reservationID)
        {
            return true;
        }

        current = current->next;
    }

    return false;
}

bool ReservationManager::hasResourceDateConflict(
    const std::string& resourceID,
    const std::string& reservationDate) const
{
    Node* current = head;

    while (current != nullptr)
    {
        if (current->data.getResourceID() == resourceID &&
            current->data.getReservationDate() == reservationDate)
        {
            return true;
        }

        current = current->next;
    }

    return false;
}

bool ReservationManager::removeReservation(
    const std::string& reservationID,
    Reservation& removedReservation)
{
    Node* current = head;
    Node* previous = nullptr;

    while (current != nullptr)
    {
        if (current->data.getReservationID() == reservationID)
        {
            removedReservation = current->data;

            if (previous == nullptr)
            {
                head = current->next;
            }
            else
            {
                previous->next = current->next;
            }

            delete current;
            return true;
        }

        previous = current;
        current = current->next;
    }

    return false;
}

bool ReservationManager::editReservation(
    const std::string& reservationID,
    const std::string& newDate)
{
    Node* current = head;

    while (current != nullptr)
    {
        if (current->data.getReservationID() == reservationID)
        {
            Node* check = head;

            while (check != nullptr)
            {
                if (check != current &&
                    check->data.getResourceID() ==
                        current->data.getResourceID() &&
                    check->data.getReservationDate() == newDate)
                {
                    return false;
                }

                check = check->next;
            }

            current->data.setReservationDate(newDate);
            return true;
        }

        current = current->next;
    }

    return false;
}

// Counts current active reservations for one resource: O(n).
int ReservationManager::countReservationsForResource(
    const std::string& resourceID) const
{
    int count = 0;
    Node* current = head;

    while (current != nullptr)
    {
        if (current->data.getResourceID() == resourceID)
        {
            count++;
        }

        current = current->next;
    }

    return count;
}

// Combines two lists that are already sorted by date.
ReservationManager::Node* ReservationManager::mergeSortedLists(
    Node* left, Node* right)
{
    Node dummy;
    dummy.next = nullptr;
    Node* tail = &dummy;

    while (left != nullptr && right != nullptr)
    {
        // Taking the left node on equal dates preserves tie order.
        if (left->data.getReservationDate() <=
            right->data.getReservationDate())
        {
            tail->next = left;
            left = left->next;
        }
        else
        {
            tail->next = right;
            right = right->next;
        }

        tail = tail->next;
    }

    if (left != nullptr)
    {
        tail->next = left;
    }
    else
    {
        tail->next = right;
    }

    return dummy.next;
}

// Manual merge sort: O(n log n) time.
ReservationManager::Node* ReservationManager::mergeSort(Node* start)
{
    if (start == nullptr || start->next == nullptr)
    {
        return start;
    }

    Node* slow = start;
    Node* fast = start->next;

    // Find the midpoint.
    while (fast != nullptr && fast->next != nullptr)
    {
        slow = slow->next;
        fast = fast->next->next;
    }

    // Separate the two halves.
    Node* right = slow->next;
    slow->next = nullptr;

    Node* sortedLeft = mergeSort(start);
    Node* sortedRight = mergeSort(right);

    return mergeSortedLists(sortedLeft, sortedRight);
}

void ReservationManager::sortByDate()
{
    head = mergeSort(head);
}


#include <fstream>
#include <sstream>

// Accept both date formats and validate the calendar date.
bool ReservationManager::normalizeDate(
    const std::string& input,
    std::string& output)
{
    if (input.size() != 10)
    {
        return false;
    }

    std::string date;

    if (input[4] == '-' && input[7] == '-')
    {
        date = input;
    }
    else if (input[2] == '/' && input[5] == '/')
    {
        date = input.substr(6, 4) + "-" +
               input.substr(0, 2) + "-" +
               input.substr(3, 2);
    }
    else
    {
        return false;
    }

    for (std::size_t i = 0; i < date.size(); i++)
    {
        if (i == 4 || i == 7)
        {
            continue;
        }

        if (date[i] < '0' || date[i] > '9')
        {
            return false;
        }
    }

    int year = std::stoi(date.substr(0, 4));
    int month = std::stoi(date.substr(5, 2));
    int day = std::stoi(date.substr(8, 2));

    if (year < 1 || month < 1 || month > 12)
    {
        return false;
    }

    int daysInMonth[] = {
        31, 28, 31, 30, 31, 30,
        31, 31, 30, 31, 30, 31
    };

    bool leapYear =
        year % 400 == 0 ||
        (year % 4 == 0 && year % 100 != 0);

    if (leapYear)
    {
        daysInMonth[1] = 29;
    }

    if (day < 1 || day > daysInMonth[month - 1])
    {
        return false;
    }

    output = date;
    return true;
}

bool ReservationManager::loadReservations(
    const std::string& filename)
{
    std::ifstream file(filename);

    if (!file.is_open())
    {
        std::cerr << "Could not open " << filename << '\n';
        return false;
    }

    std::string line;
    int lineNumber = 0;
    bool allValid = true;

    while (std::getline(file, line))
    {
        lineNumber++;

        if (!line.empty() && line.back() == '\r')
        {
            line.pop_back();
        }

        if (line.empty())
        {
            continue;
        }

        std::stringstream row(line);
        std::string fields[5];
        bool valid = true;

        for (int i = 0; i < 4; i++)
        {
            if (!std::getline(row, fields[i], '|') ||
                fields[i].empty())
            {
                valid = false;
            }
        }

        if (!std::getline(row, fields[4]) ||
            fields[4].find('|') != std::string::npos)
        {
            valid = false;
        }

        std::string date;

        if (!valid || !normalizeDate(fields[4], date))
        {
            std::cerr << "Invalid reservation line "
                      << lineNumber << '\n';
            allValid = false;
            continue;
        }

        Reservation reservation(
            fields[0], fields[1], fields[2], fields[3], date);

        if (!addReservation(reservation))
        {
            std::cerr << "Duplicate ID or date conflict on line "
                      << lineNumber << '\n';
            allValid = false;
        }
    }

    return allValid;
}
