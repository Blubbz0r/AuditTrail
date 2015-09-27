#pragma once

#include "Message.h"

#include "ActiveParticipant.h"
#include "Event.h"
#include "SOPClass.h"

#include <memory>

namespace AuditTrail
{

struct EntityParticipantObject;
enum class EventActionCode;

class InstancesAccessed : public Message
{
public:
    enum class Action
    {
        Create,
        Read,
        Update,
        Delete
    };

    InstancesAccessed(Outcome outcome, Action action);
    ~InstancesAccessed();

    std::vector<IO::Node> createNodes() const override;

    void setManipulatingPerson(ActiveParticipant manipulatingPerson);
    void setManipulatingProcess(ActiveParticipant manipulatingProcess);

    void addStudy(std::string studyInstanceUid, std::vector<SOPClass> sopClasses);

private:
    EventActionCode actionToActionCode() const;

    Outcome m_outcome;
    Action m_action;
    std::unique_ptr<ActiveParticipant> m_manipulatingPerson;
    std::unique_ptr<ActiveParticipant> m_manipulatingProcess;
    std::vector<EntityParticipantObject> m_studies;
};

}