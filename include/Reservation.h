#ifndef RESERVATION_H
#define RESERVATION_H

#include <string>

class Reservation
{
private:
    int reservationID;
    int studentID;
    std::string studentName;
    std::string resourceID;
    std::string reservationDate;

public:
    Reservation();

    Reservation(int resID, int stuID, std::string name,
                std::string resource, std::string date);

    int getReservationID() const;
    int getStudentID() const;
    std::string getStudentName() const;
    std::string getResourceID() const;
    std::string getReservationDate() const;
};

#endif