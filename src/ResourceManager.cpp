#include "ResourceManager.h"
#include "ReservationManager.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

const std::vector<Resource>& ResourceManager::getResources() const
{
    return resources;
}

bool ResourceManager::isResourceAvailableOnDate(
    const std::string& resourceID,
    const std::string& date,
    const ReservationManager& reservations) const
{
    std::string normalizedDate;
    return ReservationManager::normalizeDate(date, normalizedDate) &&
           resourceExists(resourceID) &&
           !reservations.hasResourceDateConflict(resourceID, normalizedDate);
}

void ResourceManager::displayAvailabilityForDate(
    const std::string& date,
    const ReservationManager& reservations) const
{
    std::string normalizedDate;
    if (!ReservationManager::normalizeDate(date, normalizedDate))
    {
        std::cout << "Invalid calendar date.\n";
        return;
    }
    std::cout << "\n===== RESOURCE AVAILABILITY FOR " << normalizedDate << " =====\n";
    if (resources.empty())
    {
        std::cout << "No resources loaded.\n";
        return;
    }

    std::cout << std::left << std::setw(12) << "ID"
              << std::setw(25) << "Name" << "Availability\n";
    for (const Resource& resource : resources)
    {
        std::cout << std::left << std::setw(12) << resource.getResourceID()
                  << std::setw(25) << resource.getResourceName()
                  << (isResourceAvailableOnDate(resource.getResourceID(), normalizedDate,
                                                reservations)
                          ? "Available" : "Reserved") << '\n';
    }
}

bool ResourceManager::loadResources(const std::string& filename)
{
    std::ifstream inputFile(filename);

    if (!inputFile.is_open())
    {
        std::cout << "Error: Could not open resource file." << std::endl;
        return false;
    }

    resources.clear();

    std::string line;

    while (std::getline(inputFile, line))
    {
        if (line.empty())
        {
            continue;
        }

        std::stringstream ss(line);

        std::string id;
        std::string name;
        std::string type;
        std::string status;

        std::getline(ss, id, '|');
        std::getline(ss, name, '|');
        std::getline(ss, type, '|');
        std::getline(ss, status);
	if (!status.empty() && status.back() == '\r')
	{
    	    status.pop_back();
	}

        Resource resource(id, name, type, status);

        resources.push_back(resource);
    }

    inputFile.close();

    return true;
}

void ResourceManager::displayAllResources() const
{
    if (resources.empty())
    {
        std::cout << "No resources available." << std::endl;
        return;
    }

    std::cout << std::left
              << std::setw(12) << "ID"
              << std::setw(25) << "Name"
              << std::setw(25) << "Type"
              << "Status"
              << std::endl;

    std::cout << "-----------------------------------------------------------------------"
              << std::endl;

    for (const Resource& resource : resources)
    {
        std::cout << std::left
                  << std::setw(12) << resource.getResourceID()
                  << std::setw(25) << resource.getResourceName()
                  << std::setw(25) << resource.getResourceType()
                  << resource.getAvailability()
                  << std::endl;
    }
}

void ResourceManager::displayAvailability() const
{
    if (resources.empty())
    {
        std::cout << "No resources available." << std::endl;
        return;
    }

    std::cout << std::left
              << std::setw(12) << "ID"
              << std::setw(25) << "Name"
              << "Availability"
              << std::endl;

    std::cout << "-------------------------------------------------------"
              << std::endl;

    for (const Resource& resource : resources)
    {
        std::cout << std::left
                  << std::setw(12) << resource.getResourceID()
                  << std::setw(25) << resource.getResourceName()
                  << resource.getAvailability()
                  << std::endl;
    }
}

bool ResourceManager::resourceExists(const std::string& resourceID) const
{
    for (const Resource& resource : resources)
    {
        if (resource.getResourceID() == resourceID)
        {
            return true;
        }
    }

    return false;
}

bool ResourceManager::isResourceAvailable(const std::string& resourceID) const
{
    for (const Resource& resource : resources)
    {
        if (resource.getResourceID() == resourceID)
        {
            return resource.getAvailability() == "Available";
        }
    }

    return false;
}

bool ResourceManager::setAvailability(const std::string& resourceID,
                                      const std::string& status)
{
    for (Resource& resource : resources)
    {
        if (resource.getResourceID() == resourceID)
        {
            resource.setAvailability(status);
            return true;
        }
    }

    return false;
}
