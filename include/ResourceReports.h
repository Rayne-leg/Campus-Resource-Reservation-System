#ifndef RESOURCE_REPORTS_H
#define RESOURCE_REPORTS_H

#include "ResourceManager.h"
#include "WaitingList.h"
#include <functional>
#include <iostream>
#include <string>

// The menu connects this callback to Ken's live reservation-count function.
// It must return the active reservation count for the supplied resource ID.
using ReservationCounter = std::function<int(const std::string&)>;

class ResourceReports
{
public:
    static void displayResourceUtilization(
        const ResourceManager& resources,
        const ReservationCounter& countReservations,
        std::ostream& output = std::cout);

    static void displayMostRequestedResources(
        const ResourceManager& resources,
        const ReservationCounter& countReservations,
        std::ostream& output = std::cout);

    static void displayWaitingListStatistics(
        const ResourceManager& resources,
        const WaitingList& waitingList,
        std::ostream& output = std::cout);
};

#endif
