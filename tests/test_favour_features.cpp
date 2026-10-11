#include "ResourceManager.h"
#include "ReservationManager.h"
#include "ResourceReports.h"
#include "WaitingList.h"
#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

std::string rowFor(const std::string& report, const std::string& id)
{
    std::istringstream lines(report);
    std::string line;
    while (std::getline(lines, line))
        if (line.compare(0, id.size(), id) == 0)
            return line;
    return "";
}

void testDateAvailability()
{
    ResourceManager resources;
    ReservationManager reservations;
    assert(resources.loadResources("data/resources.txt"));
    assert(resources.getResources().size() == 20);
    assert(reservations.addReservation(
        Reservation("TEST1", "S1", "One", "R101", "2026-10-11")));
    assert(!resources.isResourceAvailableOnDate("R101", "2026-10-11", reservations));
    assert(!resources.isResourceAvailableOnDate("R101", "10/11/2026", reservations));
    assert(!resources.isResourceAvailableOnDate("R101", "2026-02-30", reservations));
    assert(resources.isResourceAvailableOnDate("R101", "2026-10-12", reservations));
    assert(resources.isResourceAvailableOnDate("R102", "2026-10-11", reservations));
    assert(!resources.isResourceAvailableOnDate("UNKNOWN", "2026-10-12", reservations));

    // A legacy global status must not turn a one-date reservation into an all-date block.
    resources.setAvailability("R101", "Unavailable");
    assert(resources.isResourceAvailableOnDate("R101", "2026-10-12", reservations));
    assert(reservations.addReservation(
        Reservation("TEST2", "S2", "Two", "R101", "2026-10-12")));
    assert(!reservations.addReservation(
        Reservation("TEST3", "S3", "Three", "R101", "2026-10-12")));
    assert(!reservations.editReservation("TEST1", "2026-10-12"));
    assert(reservations.editReservation("TEST1", "2026-10-13"));
    assert(resources.isResourceAvailableOnDate("R101", "2026-10-11", reservations));
    assert(!resources.isResourceAvailableOnDate("R101", "2026-10-13", reservations));
    Reservation removed;
    assert(reservations.removeReservation("TEST2", removed));
    assert(resources.isResourceAvailableOnDate("R101", "2026-10-12", reservations));
    assert(!resources.isResourceAvailableOnDate("R101", "2026-10-13", reservations));
}

void testQueueSelectionAndCounts()
{
    WaitingList queue;
    WaitingRequest out;
    out.studentID = "UNCHANGED";
    assert(!queue.dequeueForResourceDate("R101", "2026-10-11", out));
    assert(out.studentID == "UNCHANGED");
    queue.enqueue(WaitingRequest("S1", "First", "R101", "2026-10-12"));
    queue.enqueue(WaitingRequest("S2", "Other resource", "R102", "2026-10-11"));
    queue.enqueue(WaitingRequest("S3", "First eligible", "R101", "2026-10-11"));
    queue.enqueue(WaitingRequest("S4", "Second eligible", "R101", "2026-10-11"));
    queue.enqueue(WaitingRequest("S1", "First, another date", "R101", "2026-10-13"));
    assert(queue.countWaitingRequestsForResource("R101") == 4);
    assert(queue.countWaitingStudentsForResource("R101") == 3);
    assert(queue.countWaitingStudentsForResource("R102") == 1);
    assert(queue.countWaitingStudentsForResource("R120") == 0);
    assert(queue.hasWaitingRequest("S3", "R101", "2026-10-11"));
    assert(!queue.hasWaitingRequest("S3", "R101", "2026-10-12"));

    assert(queue.peekForResourceDate("R101", "2026-10-11", out));
    assert(out.studentID == "S3");
    assert(queue.countWaitingRequestsForResource("R101") == 4);

    assert(queue.dequeueForResourceDate("R101", "2026-10-11", out));
    assert(out.studentID == "S3");
    assert(queue.dequeueForResourceDate("R101", "2026-10-11", out));
    assert(out.studentID == "S4");
    assert(!queue.dequeueForResourceDate("R101", "2026-10-11", out));
    assert(queue.dequeue(out) && out.studentID == "S1");
    assert(queue.dequeue(out) && out.studentID == "S2");
    assert(queue.dequeue(out) && out.studentID == "S1");
    assert(queue.isEmpty());

    // Remove the rear, then enqueue again to check that the rear pointer is repaired.
    queue.enqueue(WaitingRequest("A", "A", "R101", "D1"));
    queue.enqueue(WaitingRequest("B", "B", "R102", "D2"));
    assert(queue.dequeueForResourceDate("R102", "D2", out));
    queue.enqueue(WaitingRequest("C", "C", "R103", "D3"));
    assert(queue.dequeue(out) && out.studentID == "A");
    assert(queue.dequeue(out) && out.studentID == "C");
    assert(queue.isEmpty());
    queue.enqueue(WaitingRequest("D", "D", "R101", "D1"));
    assert(queue.dequeueForResourceDate("R101", "D1", out));
    assert(queue.isEmpty());
    queue.enqueue(WaitingRequest("E", "E", "R101", "D1"));
    assert(queue.dequeue(out) && out.studentID == "E");
}

void testReportsWithLiveState()
{
    ResourceManager resources;
    ReservationManager reservations;
    WaitingList waiting;
    assert(resources.loadResources("data/resources.txt"));
    std::vector<Reservation> fixtures;

    // Use the same live counting function as the production report menu.
    ReservationCounter counter = [&](const std::string& resourceID)
    {
        return reservations.countReservationsForResource(resourceID);
    };

    std::ostringstream emptyPopularity;
    ResourceReports::displayMostRequestedResources(resources, counter, emptyPopularity);
    assert(emptyPopularity.str().find("No active reservations yet") != std::string::npos);

    fixtures.emplace_back("A", "S1", "One", "R101", "2026-10-11");
    fixtures.emplace_back("B", "S2", "Two", "R101", "2026-10-12");
    fixtures.emplace_back("C", "S3", "Three", "R102", "2026-10-11");
    fixtures.emplace_back("D", "S4", "Four", "R102", "2026-10-12");
    fixtures.emplace_back("E", "S5", "Five", "R103", "2026-10-11");
    for (const Reservation& item : fixtures)
        assert(reservations.addReservation(item));

    std::ostringstream utilization;
    ResourceReports::displayResourceUtilization(resources, counter, utilization);
    for (const Resource& resource : resources.getResources())
        assert(!rowFor(utilization.str(), resource.getResourceID()).empty());
    assert(rowFor(utilization.str(), "R101").back() == '2');
    assert(rowFor(utilization.str(), "R120").back() == '0');
    assert(utilization.str().find("Total active reservations: 5") != std::string::npos);

    std::ostringstream popularity;
    ResourceReports::displayMostRequestedResources(resources, counter, popularity);
    assert(!rowFor(popularity.str(), "R101").empty());
    assert(!rowFor(popularity.str(), "R102").empty());
    assert(rowFor(popularity.str(), "R103").empty());

    Reservation removed;
    assert(reservations.removeReservation("B", removed));
    std::ostringstream changed;
    ResourceReports::displayResourceUtilization(resources, counter, changed);
    assert(rowFor(changed.str(), "R101").back() == '1');
    assert(changed.str().find("Total active reservations: 4") != std::string::npos);
    std::ostringstream changedPopularity;
    ResourceReports::displayMostRequestedResources(resources, counter, changedPopularity);
    assert(rowFor(changedPopularity.str(), "R101").empty());
    assert(!rowFor(changedPopularity.str(), "R102").empty());

    waiting.enqueue(WaitingRequest("S10", "Ten", "R101", "2026-10-11"));
    waiting.enqueue(WaitingRequest("S11", "Eleven", "R101", "2026-10-11"));
    waiting.enqueue(WaitingRequest("S10", "Ten", "R101", "2026-10-12"));
    std::ostringstream statistics;
    ResourceReports::displayWaitingListStatistics(resources, waiting, statistics);
    for (const Resource& resource : resources.getResources())
        assert(!rowFor(statistics.str(), resource.getResourceID()).empty());
    std::istringstream row(rowFor(statistics.str(), "R101"));
    std::string id, study, room, number;
    int students = 0, requests = 0;
    row >> id >> study >> room >> number >> students >> requests;
    assert(students == 2 && requests == 3);
    WaitingRequest out;
    assert(waiting.dequeueForResourceDate("R101", "2026-10-11", out));
    assert(waiting.countWaitingStudentsForResource("R101") == 2);
    assert(waiting.countWaitingRequestsForResource("R101") == 2);
    assert(waiting.dequeueForResourceDate("R101", "2026-10-11", out));
    assert(waiting.countWaitingStudentsForResource("R101") == 1);

    std::ostringstream disconnected;
    ResourceReports::displayResourceUtilization(resources, ReservationCounter(), disconnected);
    assert(disconnected.str().find("has not been connected") != std::string::npos);

    ResourceManager noResources;
    std::ostringstream noData;
    ResourceReports::displayResourceUtilization(noResources, counter, noData);
    ResourceReports::displayMostRequestedResources(noResources, counter, noData);
    ResourceReports::displayWaitingListStatistics(noResources, waiting, noData);
    assert(noData.str().find("No resources loaded") != std::string::npos);
}

int main()
{
    testDateAvailability();
    testQueueSelectionAndCounts();
    testReportsWithLiveState();
    std::cout << "PASS: date-specific availability, queue order, live report counts, ties, "
                 "zero counts, distinct students, and empty-state checks.\n";
}
