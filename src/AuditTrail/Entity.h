#pragma once

#include "Node.h"

namespace AuditTrail
{

/*!
    \brief Abstract interface for entities. An entity knows how to convert an AuditTrail message to
           our internal IO::Node representation.
*/
class Entity
{
public:
    virtual ~Entity() {}

    virtual IO::Node toNode() const = 0;
};

}