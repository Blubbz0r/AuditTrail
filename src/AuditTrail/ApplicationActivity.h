#pragma once

#include "Message.h"

#include "ActiveParticipant.h"
#include "EntityEvent.h"

namespace AuditTrail
{

enum class ApplicationEvent
{
    Started,
    Stopped
};

/*!
    \brief Describes the event of an application entity starting or stopping.
*/
class ApplicationActivity : public Message
{
public:
    ApplicationActivity(ApplicationEvent event, Outcome outcome,
                        ActiveParticipant applicationInfo);

    std::vector<IO::Node> createNodes() const override;

    void addInvolvedApplication(ActiveParticipant applicationInfo);
    void addInvolvedUser(ActiveParticipant userInfo);

private:
    ApplicationEvent m_event;
    Outcome m_outcome;
    ActiveParticipant m_applicationInfo;
    std::vector<ActiveParticipant> m_involvedApplications;
    std::vector<ActiveParticipant> m_involvedUsers;

};

}
