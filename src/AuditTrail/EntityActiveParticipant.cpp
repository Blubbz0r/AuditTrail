#include "EntityActiveParticipant.h"

namespace AuditTrail
{

EntityActiveParticipant::EntityActiveParticipant(ActiveParticipant participant)
    : m_participant(std::move(participant))
{
}

IO::Node EntityActiveParticipant::toNode() const
{
    IO::Node activeParticipant("ActiveParticipant");
    activeParticipant.addAttribute(IO::Attribute{"UserID", m_participant.userId});

    if (!m_participant.alternativeUserId().empty())
        activeParticipant.addAttribute(IO::Attribute{"AlternativeUserID", m_participant.alternativeUserId()});

    if (!m_participant.userName.empty())
        activeParticipant.addAttribute(IO::Attribute{"UserName", m_participant.userName});

    activeParticipant.addAttribute(
        IO::Attribute{"UserIsRequestor", m_participant.userIsRequestor ? "true" : "false"});

    if (m_participant.networkAccessPointType != NetworkAccessPointType::None)
    {
        activeParticipant.addAttribute(
            IO::Attribute{"NetworkAccessPointTypeCode",
                          std::to_string(static_cast<int>(m_participant.networkAccessPointType))});
    }

    if (!m_participant.networkAccessPointId.empty())
    {
        activeParticipant.addAttribute(
            IO::Attribute{"NetworkAccessPointID", m_participant.networkAccessPointId});
    }

    if (m_participant.roleIdCode.isValid())
        activeParticipant.addNode(m_participant.roleIdCode.toNode());

    return activeParticipant;
}

}