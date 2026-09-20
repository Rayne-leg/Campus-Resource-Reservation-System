#ifndef CANCELLATION_STACK_H
#define CANCELLATION_STACK_H

#include <string>

// keep srecord of one cancelled reservation
// WaitingRequest (studentID, studentName, resourceID, reservationDate)
// so the same reservation data can go from the reservation list,
// the waiting list, and this history stack without having to rename things
struct CancelledReservation
{
    std::string reservationID;
    std::string studentID;
    std::string studentName;
    std::string resourceID;
    std::string reservationDate;   // date/time the reservation was first for
    std::string cancelDate;        // date/time when the cancellation was

    CancelledReservation();
    CancelledReservation(std::string resID,
                          std::string studID,
                          std::string studName,
                          std::string resourceId,
                          std::string resDate,
                          std::string cancelledOn);
};

class CancellationStack
{
private:
    struct Node
    {
        CancelledReservation record;
        Node* next;

        Node(const CancelledReservation& value)
        {
            record = value;
            next = nullptr;
        }
    };

    Node* top;
    int count;

public:
    CancellationStack();
    ~CancellationStack();

    // puts new cancelled reservation to the top
    void push(const CancelledReservation& record);

    // removes the most recent cancellation off the stack
    // returns false and leaves removedRecord unchanged if stack is empty
    bool pop(CancelledReservation& removedRecord);

    // look at the most recent cancellation without removing it
    bool peek(CancelledReservation& record) const;

    // prints every cancelled reservation from the most recent
    void displayHistory() const;

    bool isEmpty() const;

    int getSize() const;
};

#endif
