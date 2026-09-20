#ifndef RESOURCE_H
#define RESOURCE_H

#include <string>

class Resource
{
private:
    std::string resourceID;
    std::string resourceName;
    std::string resourceType;
    std::string availability;

public:
    Resource();

    Resource(std::string id,
             std::string name,
             std::string type,
             std::string status);

    std::string getResourceID() const;
    std::string getResourceName() const;
    std::string getResourceType() const;
    std::string getAvailability() const;
};

#endif