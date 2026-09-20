#include "../include/Reservation.h"

// Default constructor
Reservation::Reservation()
{
    reservationID = 0;
    studentID = 0;
    studentName = "";
    resourceID = "";
    reservationDate = "";
}

// Constructor with information
Reservation::Reservation(int resID, int stuID, std::string name,
                         std::string resource, std::string date)
{
    reservationID = resID;
    studentID = stuID;
    studentName = name;
    resourceID = resource;
    reservationDate = date;
}

// Get reservation ID
int Reservation::getReservationID() const
{
    return reservationID;
}

// Get student ID
int Reservation::getStudentID() const
{
    return studentID;
}

// Get student name
std::string Reservation::getStudentName() const
{
    return studentName;
}

// Get resource ID
std::string Reservation::getResourceID() const
{
    return resourceID;
}

// Get reservation date
std::string Reservation::getReservationDate() const
{
    return reservationDate;
}