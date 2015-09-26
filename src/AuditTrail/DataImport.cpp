#include "DataImport.h"

#include "EntityActiveParticipant.h"

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

    for (const auto& importingUser : m_importingUsers)
        nodes.emplace_back(EntityActiveParticipant(importingUser).toNode());

    for (const auto& importingProcess : m_importingProcesses)
        nodes.emplace_back(EntityActiveParticipant(importingProcess).toNode());

    return nodes;
}

void DataImport::addImportingUser(ActiveParticipant importingUser)
{
    importingUser.roleIdCode = generateCode(CodeType::Destination);
    m_importingUsers.emplace_back(std::move(importingUser));
}

void DataImport::addImportingProcess(ActiveParticipant importingProcess)
{
    importingProcess.roleIdCode = generateCode(CodeType::Destination);
    m_importingProcesses.emplace_back(std::move(importingProcess));
}

}
