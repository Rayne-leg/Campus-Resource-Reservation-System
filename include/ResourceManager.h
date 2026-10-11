#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "Resource.h"
#include <string>
#include <vector>

class ReservationManager;

class ResourceManager
{
private:
    std::vector<Resource> resources;

public:
    // Read-only access lets reports include every resource, including zero counts.
    const std::vector<Resource>& getResources() const;

    // Booking availability is based on active reservations for the requested date.
    bool isResourceAvailableOnDate(const std::string& resourceID,
                                   const std::string& date,
                                   const ReservationManager& reservations) const;

    void displayAvailabilityForDate(const std::string& date,
                                    const ReservationManager& reservations) const;

    bool loadResources(const std::string& filename);

    void displayAllResources() const;

    void displayAvailability() const;

    bool resourceExists(const std::string& resourceID) const;

    bool isResourceAvailable(const std::string& resourceID) const;

    bool setAvailability(const std::string& resourceID,
                         const std::string& status);
};

#endif
