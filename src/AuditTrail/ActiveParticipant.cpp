#include "ActiveParticipant.h"
#include "STLUtils.h"

namespace AuditTrail
{

ActiveParticipant::ActiveParticipant(std::string userId, bool userIsRequestor)
    : userId(std::move(userId)),
      userIsRequestor(userIsRequestor),
      networkAccessPointType(NetworkAccessPointType::None)
{
}

std::string ActiveParticipant::alternativeUserId() const
{
    if (!m_aeTitles.empty())
        return "AETITLES= " + STLUtils::join(m_aeTitles, ";");
    else if (!m_alternativeUserId.empty())
        return m_alternativeUserId;
    else
        return std::string();
}

void ActiveParticipant::addAETitle(std::string aeTitle)
{
    m_aeTitles.emplace_back(std::move(aeTitle));
}

}