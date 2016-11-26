#pragma once

#include "Message.h"

#include "ActiveParticipant.h"
#include "EntityEvent.h"
#include "EntityActiveParticipant.h"
#include "Event.h"

namespace AuditTrail
{

enum class ApplicationEvent
{
    Started,
    Stopped
};

/*! Describes the event of an application entity starting or stopping. */
struct ApplicationActivity : public Message
{
    ApplicationActivity(ApplicationEvent applicationEvent, Outcome outcome,
                        ActiveParticipant appInfo);

    std::vector<IO::Node> createNodes() const override;

    EntityEvent event;
    EntityActiveParticipant applicationInfo;

    void addInvolvedApplication(ActiveParticipant applicationInfo);
    std::vector<EntityActiveParticipant> involvedApplications;

    void addInvolvedUser(ActiveParticipant userInfo);
    std::vector<EntityActiveParticipant> involvedUsers;

private:
    static ActiveParticipant applicationInfoWithRoleIdCode(ActiveParticipant appInfo);
};

}
