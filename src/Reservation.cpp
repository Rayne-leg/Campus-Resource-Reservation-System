#include "../include/Reservation.h"
Reservation::Reservation()
{
    reservationID = "";
    studentID = "";
    studentName = "";
    resourceID = "";
    reservationDate = "";
}

Reservation::Reservation(std::string resID,
                         std::string stuID,
                         std::string name,
                         std::string resource,
                         std::string date)
{
    reservationID = resID;
    studentID = stuID;
    studentName = name;
    resourceID = resource;
    reservationDate = date;
}

std::string Reservation::getReservationID() const
{
    return reservationID;
}

std::string Reservation::getStudentID() const
{
    return studentID;
}

std::string Reservation::getStudentName() const
{
    return studentName;
}

std::string Reservation::getResourceID() const
{
    return resourceID;
}

std::string Reservation::getReservationDate() const
{
    return reservationDate;
}

void Reservation::setReservationDate(const std::string& date)
{
    reservationDate = date;
}