#include "ResourceReports.h"
#include <iomanip>
#include <vector>

void ResourceReports::displayResourceUtilization(
    const ResourceManager& resources,
    const ReservationCounter& countReservations,
    std::ostream& output)
{
    output << "\n===== RESOURCE UTILIZATION =====\n";
    if (!countReservations)
    {
        output << "Reservation-count function has not been connected.\n";
        return;
    }
    if (resources.getResources().empty())
    {
        output << "No resources loaded.\n";
        return;
    }

    output << std::left << std::setw(12) << "ID"
           << std::setw(25) << "Name" << "Active reservations\n";
    int total = 0;
    for (const Resource& resource : resources.getResources())
    {
        const int count = countReservations(resource.getResourceID());
        output << std::left << std::setw(12) << resource.getResourceID()
               << std::setw(25) << resource.getResourceName() << count << '\n';
        total += count;
    }
    output << "Total active reservations: " << total << '\n';
}

void ResourceReports::displayMostRequestedResources(
    const ResourceManager& resources,
    const ReservationCounter& countReservations,
    std::ostream& output)
{
    output << "\n===== MOST REQUESTED RESOURCES =====\n";
    output << "Measured by current active reservation counts.\n";
    if (!countReservations)
    {
        output << "Reservation-count function has not been connected.\n";
        return;
    }
    if (resources.getResources().empty())
    {
        output << "No resources loaded.\n";
        return;
    }

    // Store this report's live counts once; do not keep a stale cached total.
    std::vector<int> counts;
    int maximum = 0;
    for (const Resource& resource : resources.getResources())
    {
        const int count = countReservations(resource.getResourceID());
        counts.push_back(count);
        if (count > maximum)
            maximum = count;
    }
    if (maximum == 0)
    {
        output << "No active reservations yet.\n";
        return;
    }

    output << std::left << std::setw(12) << "ID"
           << std::setw(25) << "Name" << "Active reservations\n";
    for (std::size_t i = 0; i < resources.getResources().size(); ++i)
    {
        if (counts[i] == maximum)
        {
            const Resource& resource = resources.getResources()[i];
            output << std::left << std::setw(12) << resource.getResourceID()
                   << std::setw(25) << resource.getResourceName()
                   << counts[i] << '\n';
        }
    }
    output << "All resources tied for the highest count are shown.\n";
}

void ResourceReports::displayWaitingListStatistics(
    const ResourceManager& resources,
    const WaitingList& waitingList,
    std::ostream& output)
{
    output << "\n===== WAITING-LIST STATISTICS =====\n";
    if (resources.getResources().empty())
    {
        output << "No resources loaded.\n";
        return;
    }

    output << std::left << std::setw(12) << "ID"
           << std::setw(25) << "Name"
           << std::setw(20) << "Waiting students" << "Waiting requests\n";
    for (const Resource& resource : resources.getResources())
    {
        output << std::left << std::setw(12) << resource.getResourceID()
               << std::setw(25) << resource.getResourceName()
               << std::setw(20)
               << waitingList.countWaitingStudentsForResource(resource.getResourceID())
               << waitingList.countWaitingRequestsForResource(resource.getResourceID())
               << '\n';
    }
    output << "Waiting students counts each student ID once per resource.\n"
           << "Waiting requests counts each queued date request.\n";
}
