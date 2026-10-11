#ifndef WAITING_LIST_H
#define WAITING_LIST_H

#include <string>

struct WaitingRequest
{
    std::string studentID;
    std::string studentName;
    std::string resourceID;
    std::string reservationDate;

    WaitingRequest();
    WaitingRequest(std::string id,
                   std::string name,
                   std::string resource,
                   std::string date);
};

class WaitingList
{
private:
    struct Node
    {
        WaitingRequest request;
        Node* next;

        Node(const WaitingRequest& value)
        {
            request = value;
            next = nullptr;
        }
    };

    Node* front;
    Node* rear;

public:
    WaitingList();
    ~WaitingList();

    void enqueue(const WaitingRequest& request);

    bool dequeue(WaitingRequest& removedRequest);

    // Remove the earliest matching request; other requests keep their FIFO order.
    bool dequeueForResourceDate(const std::string& resourceID,
                                const std::string& date,
                                WaitingRequest& removedRequest);

    bool peekForResourceDate(const std::string& resourceID,
                             const std::string& date,
                             WaitingRequest& request) const;

    int countWaitingRequestsForResource(const std::string& resourceID) const;

    // A student waiting for two dates is counted once in the student statistic.
    int countWaitingStudentsForResource(const std::string& resourceID) const;

    bool hasWaitingRequest(const std::string& studentID,
                           const std::string& resourceID,
                           const std::string& date) const;

    void displayWaitingList() const;

    bool isEmpty() const;
};

#endif
