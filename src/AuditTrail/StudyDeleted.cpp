#include "StudyDeleted.h"

namespace AuditTrail
{

StudyDeleted::StudyDeleted(Outcome outcome) : m_outcome(outcome)
{
}

std::vector<IO::Node> StudyDeleted::createNodes() const
{
    std::vector<IO::Node> nodes;
    return nodes;
}

}