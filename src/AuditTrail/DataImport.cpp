#include "DataImport.h"

namespace AuditTrail
{

DataImport::DataImport(Outcome outcome) : m_outcome(outcome)
{
}

std::vector<IO::Node> DataImport::createNodes() const
{
    std::vector<IO::Node> nodes;
    return nodes;
}

}
