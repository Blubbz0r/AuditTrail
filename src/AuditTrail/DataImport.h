#pragma once

#include "Message.h"

#include "EntityActiveParticipant.h"
#include "EntityEvent.h"
#include "EntityParticipantObject.h"
#include "Event.h"
#include "Media.h"
#include "SOPClass.h"

namespace AuditTrail
{

/*!
    Describes the event of importing data into an organization, implying that the data now entering
    the system was not under the control of the security domain of this organization.

    Transfer by media within an organization is often considered a data transfer rather than a data
    import event. An example of importing is creating new local instances from data on removable
    media.
*/
struct DataImport : public Message
{
    DataImport(Outcome outcome, Media sourceMedia);
    ~DataImport();

    std::vector<IO::Node> createNodes() const override;

    EntityEvent event;
    EntityActiveParticipant sourceMedium;

    void addImportingUser(ActiveParticipant importingUser);
    std::vector<EntityActiveParticipant> importingUsers;

    void addImportingProcess(ActiveParticipant importingProcess);
    std::vector<EntityActiveParticipant> importingProcesses;

    void addSource(ActiveParticipant source);
    std::vector<EntityActiveParticipant> sources;

    void addStudy(std::string studyInstanceUid, std::vector<SOPClass> sopClasses);
    std::vector<EntityParticipantObject> studies;

    void addPatient(std::string patientId, std::string patientName = std::string());
    std::vector<EntityParticipantObject> patients;

private:
    static ActiveParticipant mediumWithRoleIdCode(Media media);
};

}
