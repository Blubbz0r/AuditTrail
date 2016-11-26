#pragma once

#include "Message.h"
#include "EntityEvent.h"
#include "EntityActiveParticipant.h"
#include "Event.h"

namespace AuditTrail
{

/*!
    Describes the event of a system, such as a mobile device, intentionally
    entering or leaving the network.
*/
struct NetworkEntry : public Message
{
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

    EntityEvent event;
    EntityActiveParticipant activeParticipant;
};

}
