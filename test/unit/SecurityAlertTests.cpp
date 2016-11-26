#include <gmock/gmock.h>

#include "SecurityAlert.h"
#include "TestData.h"

using namespace AuditTrail;
using namespace IO;
using namespace Tests;
using namespace testing;

class SecurityAlertTests : public Test
{
public:
    void checkEventIdentification(const Node& eventIdentification);
    void checkEventId(const Node& eventId);
    void checkEventTypeCode(const Node& eventTypeCode);

    void checkReportingPerson(const Node& reportingPerson);

    void checkPerformingProcess(const Node& performingProcess);

    void checkAlertSubject(const Node& alertSubject);
    void checkAlertSubjectIdTypeCode(const Node& idTypeCode);
    void checkDescription(const Node& description);
};

TEST_F(SecurityAlertTests, createNodes_WithAllAttributes_ReturnsCorrectNodes)
{
    SecurityAlert securityAlert(Outcome::Success, SecurityAlert::Type::NodeAuthentication);
    securityAlert.addReportingPerson(ActiveParticipant{ User::ArbitraryUserID, false });
    securityAlert.addPerformingParticipant(ActiveParticipant{ Process::ArbitraryProcessID, false });
    securityAlert.addAlertSubject(SecurityAlert::Subject::File, Filesystem::ArbitraryFileURI);

    auto nodes = securityAlert.createNodes();

    ASSERT_THAT(nodes.size(), Eq(4));

    auto node = nodes[0];
    ASSERT_THAT(node.name(), Eq("EventIdentification"));
    checkEventIdentification(node);

    node = nodes[1];
    ASSERT_THAT(node.name(), Eq("ActiveParticipant"));
    checkReportingPerson(node);

    node = nodes[2];
    ASSERT_THAT(node.name(), Eq("ActiveParticipant"));
    checkPerformingProcess(node);

    node = nodes[3];
    ASSERT_THAT(node.name(), Eq("ParticipantObjectIdentification"));
    checkAlertSubject(node);
}

void SecurityAlertTests::checkEventIdentification(const Node& eventIdentification)
{
    ASSERT_THAT(eventIdentification.attributes().size(), Eq(3));

    auto attribute = eventIdentification.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("EventActionCode"));
    EXPECT_THAT(attribute.value, Eq("E"));

    attribute = eventIdentification.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("EventDateTime"));
    // todo: how to check the date time?

    attribute = eventIdentification.attributes().at(2);
    ASSERT_THAT(attribute.name, Eq("EventOutcomeIndicator"));
    EXPECT_THAT(attribute.value, Eq(std::to_string(static_cast<int>(Outcome::Success))));

    ASSERT_THAT(eventIdentification.nodes().size(), Eq(2));

    auto node = eventIdentification.nodes().at(0);
    ASSERT_THAT(node.name(), Eq("EventID"));
    checkEventId(node);

    node = eventIdentification.nodes().at(1);
    ASSERT_THAT(node.name(), Eq("EventTypeCode"));
    checkEventTypeCode(node);
}

void SecurityAlertTests::checkEventId(const Node& eventId)
{
    ASSERT_THAT(eventId.attributes().size(), Eq(3));

    auto attribute = eventId.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("code"));
    EXPECT_THAT(attribute.value, Eq("110113"));

    attribute = eventId.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("codeSystemName"));
    EXPECT_THAT(attribute.value, Eq("DCM"));

    attribute = eventId.attributes().at(2);
    ASSERT_THAT(attribute.name, Eq("displayName"));
    EXPECT_THAT(attribute.value, Eq("Security Alert"));
}

void SecurityAlertTests::checkEventTypeCode(const Node& eventTypeCode)
{
    ASSERT_THAT(eventTypeCode.attributes().size(), Eq(3));

    auto attribute = eventTypeCode.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("code"));
    EXPECT_THAT(attribute.value, Eq("110126"));

    attribute = eventTypeCode.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("codeSystemName"));
    EXPECT_THAT(attribute.value, Eq("DCM"));

    attribute = eventTypeCode.attributes().at(2);
    ASSERT_THAT(attribute.name, Eq("displayName"));
    EXPECT_THAT(attribute.value, Eq("Node Authentication"));
}

void SecurityAlertTests::checkReportingPerson(const Node& reportingPerson)
{
    ASSERT_THAT(reportingPerson.attributes().size(), Eq(2));

    auto attribute = reportingPerson.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("UserID"));
    EXPECT_THAT(attribute.value, Eq(User::ArbitraryUserID));

    attribute = reportingPerson.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("UserIsRequestor"));
    EXPECT_THAT(attribute.value, Eq("false"));

    ASSERT_THAT(reportingPerson.nodes().size(), Eq(0));
}

void SecurityAlertTests::checkPerformingProcess(const Node& performingProcess)
{
    ASSERT_THAT(performingProcess.attributes().size(), Eq(2));

    auto attribute = performingProcess.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("UserID"));
    EXPECT_THAT(attribute.value, Eq(Process::ArbitraryProcessID));

    attribute = performingProcess.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("UserIsRequestor"));
    EXPECT_THAT(attribute.value, Eq("false"));

    ASSERT_THAT(performingProcess.nodes().size(), Eq(0));
}

void SecurityAlertTests::checkAlertSubject(const Node& alertSubject)
{
    ASSERT_THAT(alertSubject.attributes().size(), Eq(3));

    auto attribute = alertSubject.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("ParticipantObjectID"));
    EXPECT_THAT(attribute.value, Eq(Filesystem::ArbitraryFileURI));

    attribute = alertSubject.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("ParticipantObjectTypeCode"));
    EXPECT_THAT(attribute.value, Eq("2"));

    attribute = alertSubject.attributes().at(2);
    ASSERT_THAT(attribute.name, Eq("ParticipantObjectTypeCodeRole"));
    EXPECT_THAT(attribute.value, Eq("13"));

    ASSERT_THAT(alertSubject.nodes().size(), Eq(1));

    auto node = alertSubject.nodes().at(0);
    ASSERT_THAT(node.name(), Eq("ParticipantObjectIDTypeCode"));
    checkAlertSubjectIdTypeCode(node);
}

void SecurityAlertTests::checkAlertSubjectIdTypeCode(const Node& idTypeCode)
{
    ASSERT_THAT(idTypeCode.attributes().size(), Eq(1));

    auto attribute = idTypeCode.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("code"));
    EXPECT_THAT(attribute.value, Eq("12"));
}
