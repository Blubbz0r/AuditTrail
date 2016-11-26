#include "ApplicationActivity.h"

namespace AuditTrail
{

ApplicationActivity::ApplicationActivity(ApplicationEvent applicationEvent, Outcome outcome,
                                         ActiveParticipant appInfo)
    : event(outcome, EventActionCode::Execute, generateEventID(EventIDCode::ApplicationActivity)),
      applicationInfo(applicationInfoWithRoleIdCode(std::move(appInfo)))
{
    event.eventTypeCode = generateEventTypeCode(applicationEvent == ApplicationEvent::Started
                                                    ? EventTypeCode::ApplicationStart
                                                    : EventTypeCode::ApplicationStop);
}

std::vector<IO::Node> ApplicationActivity::createNodes() const
{
    std::vector<IO::Node> nodes;

    nodes.emplace_back(event.toNode());
    nodes.emplace_back(applicationInfo.toNode());

    for (auto involvedApplication : involvedApplications)
        nodes.emplace_back(EntityActiveParticipant(involvedApplication).toNode());

    for (auto involvedUser : involvedUsers)
        nodes.emplace_back(EntityActiveParticipant(involvedUser).toNode());

    return nodes;
}

void ApplicationActivity::addInvolvedApplication(ActiveParticipant applicationInfo)
{
    applicationInfo.roleIdCode = generateRoleIDCode(RoleIDCode::ApplicationLauncher);
    involvedApplications.emplace_back(EntityActiveParticipant{ std::move(applicationInfo) });
}

void ApplicationActivity::addInvolvedUser(ActiveParticipant userInfo)
{
    userInfo.roleIdCode = generateRoleIDCode(RoleIDCode::ApplicationLauncher);
    involvedUsers.emplace_back(std::move(userInfo));
}

ActiveParticipant ApplicationActivity::applicationInfoWithRoleIdCode(ActiveParticipant appInfo)
{
    appInfo.roleIdCode = generateRoleIDCode(RoleIDCode::Application);
    return appInfo;
}

}
