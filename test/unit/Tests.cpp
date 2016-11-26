#include "gmock/gmock.h"

#include "WriterPugixml.h"
#include "ApplicationActivity.h"
#include "TestData.h"

using namespace AuditTrail;
using namespace IO;
using namespace Tests;

TEST(T, DISABLED_test)
{
    WriterPugiXml writer("C:\\ProgramData\\test.xml");

    ApplicationActivity activity(ApplicationEvent::Started, Outcome::Success,
                                  ActiveParticipant{User::ArbitraryUserID, true});
    activity.addInvolvedApplication(ActiveParticipant{Process::ArbitraryProcessID, false});
    activity.addInvolvedUser(ActiveParticipant{ "Test", false });
    writer.write(activity.createNodes());
}
