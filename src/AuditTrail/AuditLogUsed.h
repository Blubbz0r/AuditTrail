#pragma once

#include "Message.h"

#include "ActiveParticipant.h"
#include "Event.h"

#include <memory>

namespace AuditTrail
{

/*!
    \brief Describes the event of a person or process reading a log of audit trail information.
*/
class AuditLogUsed : public Message
{
public:
    AuditLogUsed(Outcome outcome, std::string auditLogUri);

    std::vector<IO::Node> createNodes() const override;

    void setAccessingApplication(ActiveParticipant accessingApplication);
    void setAccessingUser(ActiveParticipant accessingUser);

private:
    Outcome m_outcome;
    std::unique_ptr<ActiveParticipant> m_accessingApplication;
    std::unique_ptr<ActiveParticipant> m_accessingUser;
    std::string m_auditLogUri;
};

}
