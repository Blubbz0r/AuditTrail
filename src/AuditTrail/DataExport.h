#pragma once

#include "Message.h"

#include "ActiveParticipant.h"
#include "Event.h"
#include "Media.h"
#include "SOPClass.h"

#include <memory>

namespace AuditTrail
{

struct EntityParticipantObject;

/*!
    \brief  Describes the event of exporting data from a system, meaning that the data is leaving
            control of the system's security domain (e.g. printing to paper, recording on film,
            conversion to another format for storage in an EHR, writing to removable media, sending
            via e-mail).
*/
class DataExport : public Message
{
public:
    DataExport(Outcome outcome, MediaType mediaType);
    ~DataExport();

    std::vector<IO::Node> createNodes() const override;

    void addRemoteUser(ActiveParticipant remoteUser);
    void addRemoteProcess(ActiveParticipant remoteProcess);

    void setExportingUser(ActiveParticipant exportingUser);
    void setExportingProcess(ActiveParticipant exportingProcess);

    void setMediaLabel(std::string mediaLabel) { m_mediaLabel = std::move(mediaLabel); }

    void addStudy(std::string studyInstanceUid, std::vector<SOPClass> sopClasses);
    void addPatient(std::string patientId, std::string patientName = std::string());

private:
    Outcome m_outcome;
    std::vector<ActiveParticipant> m_remoteUsersAndProcesses;
    std::unique_ptr<ActiveParticipant> m_exportingUser;
    std::unique_ptr<ActiveParticipant> m_exportingProcess;
    MediaType m_mediaType;
    std::string m_mediaLabel;
    std::vector<EntityParticipantObject> m_studies;
    std::vector<std::pair<std::string, std::string>> m_patients;
};

}