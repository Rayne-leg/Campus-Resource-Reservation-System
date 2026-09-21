Campus Resource Reservation System
CSCE 2110 – Project 1, Milestone 1

Project Overview

The Campus Resource Reservation System is a C++ menu-driven program used to manage university resources and student reservations.

The system uses:
- A vector to store campus resources
- One linked list to store all active reservations
- A queue to manage waiting-list requests
- A stack to store cancellation history

Milestone 1 Features

Resource Management
- Loads resource information from data/resources.txt
- Stores resources in a vector
- Displays all resources
- Displays resource availability
- Validates resource IDs
- Checks and updates resource availability

Reservation Management
- Creates reservations
- Cancels reservations
- Displays active reservations
- Searches for reservations
- Edits reservation dates
- Prevents duplicate reservation IDs
- Checks for resource/date conflicts

Active Reservation Linked List
- Uses one linked list for all active reservations
- Supports insertion
- Supports removal
- Supports traversal and display
- Supports searching
- Includes memory cleanup

Waiting List Queue
- Adds students when a resource is unavailable
- Processes students in FIFO order
- Displays the waiting list
- Automatically promotes the next matching student when a reservation is cancelled
- Handles an empty waiting list

Cancellation History Stack
- Stores cancelled reservations
- Displays cancellation history
- Restores the most recently cancelled reservation
- Handles an empty cancellation stack

Project Structure

include/
Contains header files for project classes and data structures.

src/
Contains C++ implementation files and the main program.

data/
Contains resources.txt, which stores the campus resource information.

Compilation

The project was compiled and tested on the UNT CSE CELL machines using C++17.

Example compile command:

g++ -std=c++17 -Iinclude src/Resource.cpp src/ResourceManager.cpp src/WaitingList.cpp src/Reservation.cpp src/ReservationManager.cpp src/CancellationStack.cpp src/main.cpp -o campus_reservation

Run command:

./campus_reservation

Team Members

Favour
- Resource Management
- Waiting List Queue
- Final integration, testing, and documentation

Ken
- Reservation Management
- Active Reservation Linked List

Ademola
- Cancellation History Stack
- Main Program Integration

Testing

The completed Milestone 1 program was tested for:
- Resource file loading
- Resource display
- Resource availability display and updates
- Reservation creation
- Reservation cancellation
- Active reservation display
- Resource/date conflict validation
- Duplicate reservation ID validation
- Waiting-list FIFO behavior
- Automatic waiting-list promotion
- Empty waiting-list handling
- Cancellation history
- Undo cancellation
- Empty cancellation-stack handling
- Invalid menu input
- Invalid resource IDs

The full integrated program compiled and ran successfully on the UNT CSE CELL machines.

GitHub Repository

https://github.com/Rayne-leg/Campus-Resource-Reservation-System