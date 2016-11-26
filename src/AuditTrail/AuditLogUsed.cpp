#include "AuditLogUsed.h"

namespace AuditTrail
{

AuditLogUsed::AuditLogUsed(Outcome outcome, std::string auditLogUri)
    : event(outcome, EventActionCode::Read, generateEventID(EventIDCode::AuditLogUsed)),
      auditLog(EntityParticipantObject::Type::SystemObject,
               EntityParticipantObject::Role::SecurityResource,
               generateParticipantObjectIDTypeCode(ParticipantObjectIDTypeCode::URI),
               std::move(auditLogUri)),
      accessingApplication(nullptr),
      accessingUser(nullptr)
{
    auditLog.objectNameOrQuery = "Security Audit Log";
}

std::vector<IO::Node> AuditLogUsed::createNodes() const
{
    std::vector<IO::Node> nodes;

    nodes.emplace_back(event.toNode());

    if (accessingApplication)
        nodes.emplace_back(accessingApplication->toNode());

    if (accessingUser)
        nodes.emplace_back(accessingUser->toNode());

    nodes.emplace_back(auditLog.toNode());

    return nodes;
}

void AuditLogUsed::setAccessingApplication(ActiveParticipant app)
{
    accessingApplication = std::make_unique<EntityActiveParticipant>(std::move(app));
}

void AuditLogUsed::setAccessingUser(ActiveParticipant user)
{
    accessingUser = std::make_unique<EntityActiveParticipant>(std::move(user));
}

}