#pragma once

#include "Message.h"
#include "ActiveParticipant.h"
#include "Event.h"

namespace AuditTrail
{

/*!
    Describes the event of a system, such as a mobile device, intentionally
    entering or leaving the network.
*/
class NetworkEntry : public Message
{
public:
    enum class Action
    {
        Attach,
        Detach
    };

    /*!
        \param  node - Node or system entering/leaving the network.
                       Note that ActiveParticipant::userIsRequestor shall be false.
    */
    NetworkEntry(Outcome outcome, Action action, ActiveParticipant node);

    std::vector<IO::Node> createNodes() const override;

private:
    Outcome m_outcome;
    Action m_action;
    ActiveParticipant m_activeParticipant;
};

}
