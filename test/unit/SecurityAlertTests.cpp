#include <gmock/gmock.h>

#include "SecurityAlert.h"
#include "CodedValueTypeTests.h"
#include "TestData.h"

using namespace AuditTrail;
using namespace IO;
using namespace Tests;
using namespace testing;

class SecurityAlertTests : public Test
{
public:
    void checkReportingPerson(const Node& reportingPerson);

    void checkPerformingProcess(const Node& performingProcess);

    void checkAlertSubject(const Node& alertSubject);
};

TEST_F(SecurityAlertTests, createNodes_WithAllAttributes_ReturnsCorrectNodes)
{
    SecurityAlert securityAlert(Outcome::Success, SecurityAlert::Type::NodeAuthentication);
    securityAlert.addReportingPerson(ActiveParticipant{ User::ArbitraryUserID, false });
    securityAlert.addPerformingParticipant(ActiveParticipant{ Process::ArbitraryProcessID, false });
    securityAlert.addAlertSubject(SecurityAlert::Subject::File, Filesystem::ArbitraryFileURI);

    auto nodes = securityAlert.createNodes();

    ASSERT_THAT(nodes.size(), Eq(4));

    auto eventIdentification = nodes[0];
    ASSERT_THAT(eventIdentification.name(), Eq("EventIdentification"));
    checkEventIdentification(eventIdentification, "E", Outcome::Success);

    ASSERT_THAT(eventIdentification.nodes().size(), Eq(2));

    auto eventId = eventIdentification.nodes().at(0);
    ASSERT_THAT(eventId.name(), Eq("EventID"));
    checkCodedValueType(eventId, "110113", "Security Alert");

    auto eventTypeCode = eventIdentification.nodes().at(1);
    ASSERT_THAT(eventTypeCode.name(), Eq("EventTypeCode"));
    checkCodedValueType(eventTypeCode, "110126", "Node Authentication");

    auto node = nodes[1];
    ASSERT_THAT(node.name(), Eq("ActiveParticipant"));
    checkReportingPerson(node);

    node = nodes[2];
    ASSERT_THAT(node.name(), Eq("ActiveParticipant"));
    checkPerformingProcess(node);

    node = nodes[3];
    ASSERT_THAT(node.name(), Eq("ParticipantObjectIdentification"));
    checkAlertSubject(node);
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
    checkCodedValueType(node, "12");
}
