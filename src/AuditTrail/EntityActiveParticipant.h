#pragma once

#include "ActiveParticipant.h"
#include "Entity.h"

namespace AuditTrail
{

class EntityActiveParticipant : public Entity
{
public:
    EntityActiveParticipant(ActiveParticipant participant);

    IO::Node toNode() const override;

private:
    ActiveParticipant m_participant;
};

}