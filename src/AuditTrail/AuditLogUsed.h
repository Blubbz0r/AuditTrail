#pragma once

#include "Message.h"

#include "ActiveParticipant.h"
#include "EntityActiveParticipant.h"
#include "EntityEvent.h"
#include "Event.h"

#include <memory>
#include "EntityParticipantObject.h"

namespace AuditTrail
{

/*! Describes the event of a person or process reading a log of audit trail information. */
struct AuditLogUsed : public Message
{
    AuditLogUsed(Outcome outcome, std::string auditLogUri);

    std::vector<IO::Node> createNodes() const override;

    EntityEvent event;

    void setAccessingApplication(ActiveParticipant app);
    std::unique_ptr<EntityActiveParticipant> accessingApplication;

    void setAccessingUser(ActiveParticipant user);
    std::unique_ptr<EntityActiveParticipant> accessingUser;

    EntityParticipantObject auditLog;
};

}
