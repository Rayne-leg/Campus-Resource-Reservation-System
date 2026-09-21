#include "Resource.h"

Resource::Resource()
{
    resourceID = "";
    resourceName = "";
    resourceType = "";
    availability = "";
}

Resource::Resource(std::string id,
                   std::string name,
                   std::string type,
                   std::string status)
{
    resourceID = id;
    resourceName = name;
    resourceType = type;
    availability = status;
}

std::string Resource::getResourceID() const
{
    return resourceID;
}

std::string Resource::getResourceName() const
{
    return resourceName;
}

std::string Resource::getResourceType() const
{
    return resourceType;
}

std::string Resource::getAvailability() const
{
    return availability;
} 
void Resource::setAvailability(const std::string& status)
{
    availability = status;
}