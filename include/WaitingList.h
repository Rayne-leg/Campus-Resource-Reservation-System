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

    void displayWaitingList() const;

    bool isEmpty() const;
};

#endif 