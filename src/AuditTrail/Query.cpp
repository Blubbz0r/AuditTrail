#include "Query.h"

namespace AuditTrail
{

Query::Query(Outcome outcome, ActiveParticipant queryingProcess, ActiveParticipant queriedProcess,
             std::string queriedSOPClassUID, std::string base64Dataset)
    : event(outcome, EventActionCode::Execute, generateEventID(EventIDCode::Query)),
      queryingProcess(std::move(queryingProcess)),
      queriedProcess(std::move(queriedProcess)),
      queriedSop(EntityParticipantObject::Type::SystemObject, EntityParticipantObject::Role::Report,
                 generateParticipantObjectIDTypeCode(ParticipantObjectIDTypeCode::SOPClassUID),
                 std::move(queriedSOPClassUID))
{
    this->queryingProcess.participant.roleIdCode = generateRoleIDCode(RoleIDCode::Source);
    this->queriedProcess.participant.roleIdCode = generateRoleIDCode(RoleIDCode::Destination);
    queriedSop.objectNameOrQuery = std::move(base64Dataset);
}

std::vector<IO::Node> Query::createNodes() const
{
    std::vector<IO::Node> nodes;

    nodes.emplace_back(event.toNode());

    nodes.emplace_back(queryingProcess.toNode());
    nodes.emplace_back(queriedProcess.toNode());

    for (const auto& otherParticipant : otherParticipants)
        nodes.emplace_back(otherParticipant.toNode());

    return nodes;
}

void Query::addOtherParticipant(ActiveParticipant otherParticipant)
{
    otherParticipants.emplace_back(EntityActiveParticipant(std::move(otherParticipant)));
}

}
