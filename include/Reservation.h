#ifndef RESERVATION_H
#define RESERVATION_H

#include <string>

class Reservation
{
private:
    std::string reservationID;
    std::string studentID;
    std::string studentName;
    std::string resourceID;
    std::string reservationDate;

public:
    Reservation();

    Reservation(std::string resID,
                std::string stuID,
                std::string name,
                std::string resource,
                std::string date);

    std::string getReservationID() const;
    std::string getStudentID() const;
    std::string getStudentName() const;
    std::string getResourceID() const;
    std::string getReservationDate() const;

    void setReservationDate(const std::string& date);
};

#endif