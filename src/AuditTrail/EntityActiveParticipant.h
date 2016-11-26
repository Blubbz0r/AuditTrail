#pragma once

#include "ActiveParticipant.h"
#include "Entity.h"

namespace AuditTrail
{

struct EntityActiveParticipant : public Entity
{
public:
    explicit EntityActiveParticipant(ActiveParticipant participant);

    IO::Node toNode() const override;

    ActiveParticipant participant;
};

}