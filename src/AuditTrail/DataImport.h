#pragma once

#include "Message.h"

#include "ActiveParticipant.h"
#include "Event.h"
#include "Media.h"
#include "SOPClass.h"

namespace AuditTrail
{

struct EntityParticipantObject;

/*!
    \brief  Describes the event of importing data into an organization, implying that the data now
            entering the system was not under the control of the security domain of this
            organization.

            Transfer by media within an organization is often considered a data transfer rather than
            a data import event. An example of importing is creating new local instances from data
            on removable media.
*/
class DataImport : public Message
{
public:
    DataImport(Outcome outcome, Media sourceMedia);
    ~DataImport();

    std::vector<IO::Node> createNodes() const override;

    void addImportingUser(ActiveParticipant importingUser);
    void addImportingProcess(ActiveParticipant importingProcess);

    void addSource(ActiveParticipant source);

    void addStudy(std::string studyInstanceUid, std::vector<SOPClass> sopClasses);

    void addPatient(std::string patientId, std::string patientName = std::string());

private:
    Outcome m_outcome;
    std::vector<ActiveParticipant> m_importingUsers;
    std::vector<ActiveParticipant> m_importingProcesses;
    Media m_sourceMedia;
    std::vector<ActiveParticipant> m_sources;
    std::vector<EntityParticipantObject> m_studies;
    std::vector<std::pair<std::string, std::string>> m_patients;
};

}