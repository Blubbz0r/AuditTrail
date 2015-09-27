#include "InstancesAccessed.h"

namespace AuditTrail
{

InstancesAccessed::InstancesAccessed(Outcome outcome, Action action)
    : m_outcome(outcome), m_action(action)
{
}

std::vector<IO::Node> InstancesAccessed::createNodes() const
{
    std::vector<IO::Node> nodes;
    return nodes;
}

}