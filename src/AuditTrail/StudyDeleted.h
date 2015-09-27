#pragma once

#include "Message.h"

#include "ActiveParticipant.h"
#include "Event.h"

#include <memory>

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

    void setDeletingPerson(ActiveParticipant deletingPerson);
    void setDeletingProcess(ActiveParticipant deletingProcess);

private:
    Outcome m_outcome;
    std::unique_ptr<ActiveParticipant> m_deletingPerson;
    std::unique_ptr<ActiveParticipant> m_deletingProcess;
};

}