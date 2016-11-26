#include "EntityActiveParticipant.h"

namespace AuditTrail
{

EntityActiveParticipant::EntityActiveParticipant(ActiveParticipant participant)
    : participant(std::move(participant))
{
}

IO::Node EntityActiveParticipant::toNode() const
{
    IO::Node activeParticipant("ActiveParticipant");
    activeParticipant.addAttribute(IO::Attribute{"UserID", participant.userId});

    if (!participant.alternativeUserId().empty())
        activeParticipant.addAttribute(IO::Attribute{"AlternativeUserID", participant.alternativeUserId()});

    if (!participant.userName.empty())
        activeParticipant.addAttribute(IO::Attribute{"UserName", participant.userName});

    activeParticipant.addAttribute(
        IO::Attribute{"UserIsRequestor", participant.userIsRequestor ? "true" : "false"});

    if (participant.networkAccessPointType != NetworkAccessPointType::None)
    {
        activeParticipant.addAttribute(
            IO::Attribute{"NetworkAccessPointTypeCode",
                          std::to_string(static_cast<int>(participant.networkAccessPointType))});
    }

    if (!participant.networkAccessPointId.empty())
    {
        activeParticipant.addAttribute(
            IO::Attribute{"NetworkAccessPointID", participant.networkAccessPointId});
    }

    if (participant.roleIdCode.isValid())
        activeParticipant.addNode(participant.roleIdCode.toNode());

    return activeParticipant;
}

}