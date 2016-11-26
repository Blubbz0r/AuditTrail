#include "Query.h"
#include "EntityActiveParticipant.h"
#include "EntityEvent.h"
#include "EntityParticipantObject.h"

namespace AuditTrail
{

Query::Query(Outcome outcome, ActiveParticipant queryingProcess, ActiveParticipant queriedProcess,
             std::string queriedSOPClassUID, std::string base64Dataset)
    : m_outcome(outcome),
      m_queryingProcess(std::move(queryingProcess)),
      m_queriedProcess(std::move(queriedProcess)),
      m_queriedSOPClassUID(std::move(queriedSOPClassUID)),
      m_base64Dataset(std::move(base64Dataset))
{
    m_queryingProcess.roleIdCode = generateRoleIDCode(RoleIDCode::Source);
    m_queriedProcess.roleIdCode = generateRoleIDCode(RoleIDCode::Destination);
}

std::vector<IO::Node> Query::createNodes() const
{
    std::vector<IO::Node> nodes;

    EntityEvent event(m_outcome, EventActionCode::Execute, generateEventID(EventIDCode::Query));
    nodes.emplace_back(event.toNode());

    nodes.emplace_back(EntityActiveParticipant(m_queryingProcess).toNode());
    nodes.emplace_back(EntityActiveParticipant(m_queriedProcess).toNode());

    for (const auto& otherParticipant : m_otherParticipants)
        nodes.emplace_back(EntityActiveParticipant(otherParticipant).toNode());

    EntityParticipantObject queriedSOP(
        EntityParticipantObject::Type::SystemObject, EntityParticipantObject::Role::Report,
        generateParticipantObjectIDTypeCode(ParticipantObjectIDTypeCode::SOPClassUID),
        m_queriedSOPClassUID);

    queriedSOP.objectNameOrQuery = m_base64Dataset;

    return nodes;
}

void Query::addOtherParticipant(ActiveParticipant otherParticipant)
{
    m_otherParticipants.emplace_back(std::move(otherParticipant));
}

}
