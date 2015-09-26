#include "DataImport.h"

#include "EntityActiveParticipant.h"

namespace AuditTrail
{

DataImport::DataImport(Outcome outcome, MediaType sourceMedia)
    : m_outcome(outcome), m_sourceMedia(sourceMedia)
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

    std::string mediaId = mediaTypeToString(m_sourceMedia);
    if (!m_mediaLabel.empty())
        mediaId += " " + m_mediaLabel;

    ActiveParticipant sourceMedia(mediaId, false);
    sourceMedia.roleIdCode = generateCode(CodeType::SourceMedia);
    nodes.emplace_back(EntityActiveParticipant(sourceMedia).toNode());

    for (const auto& source : m_sources)
        nodes.emplace_back(EntityActiveParticipant(source).toNode());

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

void DataImport::addSource(ActiveParticipant source)
{
    source.roleIdCode = generateCode(CodeType::Source);
    m_sources.emplace_back(std::move(source));
}

void DataImport::addStudy(std::string studyInstanceUid, std::vector<SOPClass> sopClasses)
{

}

}
