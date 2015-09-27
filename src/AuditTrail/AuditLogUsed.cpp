#include "AuditLogUsed.h"

#include "EntityActiveParticipant.h"
#include "EntityEvent.h"
#include "EntityParticipantObject.h"

namespace AuditTrail
{

AuditLogUsed::AuditLogUsed(Outcome outcome, std::string auditLogUri)
    : m_outcome(outcome),
      m_accessingApplication(nullptr),
      m_accessingUser(nullptr),
      m_auditLogUri(std::move(auditLogUri))
{
}

std::vector<IO::Node> AuditLogUsed::createNodes() const
{
    std::vector<IO::Node> nodes;

    EntityEvent event(m_outcome, EventActionCode::Read, generateCode(CodeType::AuditLogUsed));
    nodes.emplace_back(event.toNode());

    if (m_accessingApplication)
        nodes.emplace_back(EntityActiveParticipant(*m_accessingApplication).toNode());
    if (m_accessingUser)
        nodes.emplace_back(EntityActiveParticipant(*m_accessingUser).toNode());

    EntityParticipantObject auditLog(EntityParticipantObject::Type::SystemObject,
                                     EntityParticipantObject::Role::SecurityResource,
                                     generateCode(CodeType::URI), m_auditLogUri);
    auditLog.objectNameOrQuery = "Security Audit Log";

    nodes.emplace_back(auditLog.toNode());

    return nodes;
}

void AuditLogUsed::setAccessingApplication(ActiveParticipant accessingApplication)
{
    m_accessingApplication = std::make_unique<ActiveParticipant>(accessingApplication);
}

void AuditLogUsed::setAccessingUser(ActiveParticipant accessingUser)
{
    m_accessingUser = std::make_unique<ActiveParticipant>(accessingUser);
}

}