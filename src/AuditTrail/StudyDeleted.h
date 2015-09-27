#pragma once

#include "Message.h"

#include "Event.h"

namespace AuditTrail
{

/*!
    \brief  Describes the event of deletion of one or more studies and all associated SOP instances
            in a single action.
*/
class StudyDeleted : public Message
{
public:
    StudyDeleted(Outcome outcome);

    std::vector<IO::Node> createNodes() const override;

private:
    Outcome m_outcome;
};

}