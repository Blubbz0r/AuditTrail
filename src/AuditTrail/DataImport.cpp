#include "DataImport.h"

namespace AuditTrail
{

DataImport::DataImport(Outcome outcome) : m_outcome(outcome)
{
}

std::vector<IO::Node> DataImport::createNodes() const
{
    std::vector<IO::Node> nodes;

    EntityEvent event(m_outcome, EventActionCode::Create, generateCode(CodeType::Import));
    nodes.emplace_back(event.toNode());

    return nodes;
}

}
