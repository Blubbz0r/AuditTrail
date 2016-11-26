#pragma once

#include "Message.h"
#include "EntityActiveParticipant.h"
#include "EntityEvent.h"
#include "Event.h"

#include <memory>

namespace AuditTrail
{
/*!
    Describes the event that a user has attempted to log on or log off.
    This report can be made regardless of whether the attempt was successful or not. No Participant
    Objects are needed for this message.

    \note   The user usually has ActiveParticpant::userIsRequestor TRUE, but in the case of a logout
            timer, the Node might be the ActiveParticpant::userIsRequestor.
*/
struct UserAuthentication : public Message
{
    enum class Type
    {
        Login,
        Logout
    };

    UserAuthentication(Outcome outcome, Type type, ActiveParticipant authenticatedPerson);

    std::vector<IO::Node> createNodes() const override;

    EntityEvent event;
    EntityActiveParticipant authenticatedPerson;

    void setAuthenticatingSystem(ActiveParticipant system);
    std::unique_ptr<EntityActiveParticipant> authenticatingSystem;
};

}
