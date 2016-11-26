#pragma once

#include "Message.h"
#include "ActiveParticipant.h"
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
class UserAuthentication : public Message
{
public:
    enum class Type
    {
        Login,
        Logout
    };

    UserAuthentication(Outcome outcome, Type type, ActiveParticipant authenticatedPerson);

    std::vector<IO::Node> createNodes() const override;

    void setAuthenticatingSystem(ActiveParticipant authenticatingSystem);

private:
    Outcome m_outcome;
    Type m_type;
    ActiveParticipant m_authenticatedPerson;
    std::unique_ptr<ActiveParticipant> m_authenticatingSystem;
};

}
