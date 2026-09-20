#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "Resource.h"
#include <string>
#include <vector>

class ResourceManager
{
private:
    std::vector<Resource> resources;

public:
    bool loadResources(const std::string& filename);

    void displayAllResources() const;

    void displayAvailability() const;

    bool resourceExists(const std::string& resourceID) const;

    bool isResourceAvailable(const std::string& resourceID) const;
};

#endif