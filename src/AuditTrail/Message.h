#pragma once

#include "Node.h"

namespace AuditTrail
{

/*!
    \brief  Abstract interface for messages. Messages encapsulate a complete AuditTrail message and
            know how to convert this message to our internal IO::Node representation.
*/
class Message
{
public:
    virtual ~Message() {}

    virtual std::vector<IO::Node> createNodes() const = 0;
};

}