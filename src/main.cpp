#include <iostream>
#include "../include/Reservation.h"

int main()
{
    Reservation r1(301, 1001, "Alice Smith", "R205", "09/20/2026");

    std::cout << "Reservation ID: " << r1.getReservationID() << std::endl;
    std::cout << "Student ID: " << r1.getStudentID() << std::endl;
    std::cout << "Student Name: " << r1.getStudentName() << std::endl;
    std::cout << "Resource ID: " << r1.getResourceID() << std::endl;
    std::cout << "Reservation Date: " << r1.getReservationDate() << std::endl;

    return 0;
}