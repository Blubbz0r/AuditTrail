#include "ApplicationActivity.h"

#include "EntityActiveParticipant.h"
#include "EntityEvent.h"

namespace AuditTrail
{

ApplicationActivity::ApplicationActivity(ApplicationEvent event, Outcome outcome,
                                         ActiveParticipant applicationInfo)
    : m_event(event), m_outcome(outcome), m_applicationInfo(std::move(applicationInfo))
{
    m_applicationInfo.roleIdCode = generateCode(CodeType::Application);
}

std::vector<IO::Node> ApplicationActivity::createNodes() const
{
    std::vector<IO::Node> nodes;

    EntityEvent event(m_outcome, EventActionCode::Execute,
                      generateCode(CodeType::ApplicationActivity));
    event.eventTypeCode
        = generateCode(m_event == ApplicationEvent::Started ? CodeType::ApplicationStart
                                                             : CodeType::ApplicationStop);
    nodes.emplace_back(event.toNode());

    nodes.emplace_back(EntityActiveParticipant(m_applicationInfo).toNode());

    for (auto involvedApplication : m_involvedApplications)
    {
        involvedApplication.roleIdCode = generateCode(CodeType::ApplicationLauncher);
        nodes.emplace_back(EntityActiveParticipant(involvedApplication).toNode());
    }

    for (auto involvedUser : m_involvedUsers)
    {
        involvedUser.roleIdCode = generateCode(CodeType::ApplicationLauncher);
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
