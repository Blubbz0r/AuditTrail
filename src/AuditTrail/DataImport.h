#pragma once

#include "Message.h"

#include "ActiveParticipant.h"
#include "EntityEvent.h"
#include "Media.h"

namespace AuditTrail
{

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
    DataImport(Outcome outcome, MediaType sourceMedia);

    std::vector<IO::Node> createNodes() const override;

    void addImportingUser(ActiveParticipant importingUser);
    void addImportingProcess(ActiveParticipant importingProcess);

private:
    Outcome m_outcome;
    std::vector<ActiveParticipant> m_importingUsers;
    std::vector<ActiveParticipant> m_importingProcesses;
    MediaType m_sourceMedia;
};

}