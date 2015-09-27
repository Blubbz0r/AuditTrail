#include "ApplicationActivity.h"

#include "EntityActiveParticipant.h"
#include "EntityEvent.h"

namespace AuditTrail
{

ApplicationActivity::ApplicationActivity(ApplicationEvent event, Outcome outcome,
                                         ActiveParticipant applicationInfo)
    : m_event(event), m_outcome(outcome), m_applicationInfo(std::move(applicationInfo))
{
    m_applicationInfo.roleIdCode = generateRoleIDCode(RoleIDCode::Application);
}

std::vector<IO::Node> ApplicationActivity::createNodes() const
{
    std::vector<IO::Node> nodes;

    EntityEvent event(m_outcome, EventActionCode::Execute,
                      generateEventID(EventIDCode::ApplicationActivity));
    event.eventTypeCode = generateEventTypeCode(m_event == ApplicationEvent::Started
                                                    ? EventTypeCode::ApplicationStart
                                                    : EventTypeCode::ApplicationStop);
    nodes.emplace_back(event.toNode());

    nodes.emplace_back(EntityActiveParticipant(m_applicationInfo).toNode());

    for (auto involvedApplication : m_involvedApplications)
    {
        involvedApplication.roleIdCode = generateRoleIDCode(RoleIDCode::ApplicationLauncher);
        nodes.emplace_back(EntityActiveParticipant(involvedApplication).toNode());
    }

    for (auto involvedUser : m_involvedUsers)
    {
        involvedUser.roleIdCode = generateRoleIDCode(RoleIDCode::ApplicationLauncher);
        nodes.emplace_back(EntityActiveParticipant(involvedUser).toNode());
    }

    return nodes;
}

void ApplicationActivity::addInvolvedApplication(ActiveParticipant applicationInfo)
{
    m_involvedApplications.emplace_back(std::move(applicationInfo));
}

void ApplicationActivity::addInvolvedUser(ActiveParticipant userInfo)
{
    m_involvedUsers.emplace_back(std::move(userInfo));
}

}
