#pragma once

#include "Message.h"

#include "ActiveParticipant.h"
#include "EntityActiveParticipant.h"
#include "EntityEvent.h"
#include "EntityParticipantObject.h"
#include "Event.h"
#include "Media.h"
#include "SOPClass.h"

#include <memory>

namespace AuditTrail
{

struct EntityParticipantObject;

/*!
    Describes the event of exporting data from a system, meaning that the data is leaving control of
    the system's security domain (e.g. printing to paper, recording on film, conversion to another
    format for storage in an EHR, writing to removable media, sending via e-mail).
*/
struct DataExport : public Message
{
    DataExport(Outcome outcome, MediaType mediaType);
    ~DataExport();

    std::vector<IO::Node> createNodes() const override;

    EntityEvent event;
    EntityActiveParticipant media;

    void addRemoteUser(ActiveParticipant remoteUser);
    void addRemoteProcess(ActiveParticipant remoteProcess);
    std::vector<EntityActiveParticipant> remoteUsersAndProcesses;

    void setExportingUser(ActiveParticipant user);
    std::unique_ptr<EntityActiveParticipant> exportingUser;

    void setExportingProcess(ActiveParticipant process);
    std::unique_ptr<EntityActiveParticipant> exportingProcess;

    void setMediaLabel(std::string mediaLabel);

    void addStudy(std::string studyInstanceUid, std::vector<SOPClass> sopClasses);
    std::vector<EntityParticipantObject> studies;

    void addPatient(std::string patientId, std::string patientName = std::string());
    std::vector<std::pair<std::string, std::string>> patients;

private:
    static std::string mediaId(MediaType mediaType, const std::string& label = std::string());

    MediaType m_mediaType;
};

}
