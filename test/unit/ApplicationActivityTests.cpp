#include "gmock/gmock.h"

#include "ApplicationActivity.h"
#include "CodedValueTypeTests.h"
#include "TestData.h"

using namespace AuditTrail;
using namespace IO;
using namespace Tests;
using namespace testing;

class ApplicationActivityTests : public Test
{
public:
    void checkActiveParticipantApplication(const Node& activeParticipant);

    void checkActiveParticipantInvolvedApplication(const Node& activeParticipant);
    void checkActiveParticipantInvolvedUser(const Node& activeParticipant);
};

TEST_F(ApplicationActivityTests, createNodes_WithAllAttributes_ReturnsCorrectNodes)
{
    ActiveParticipant applicationInfo(Process::ArbitraryProcessID, false);
    applicationInfo.addAETitle(DICOM::ArbitraryAETitle);
    applicationInfo.addAETitle(DICOM::ArbitraryAETitle2);
    applicationInfo.userName = Process::ArbitraryProcessName;
    applicationInfo.networkAccessPointType = NetworkAccessPointType::IPAddress;
    applicationInfo.networkAccessPointId = Machine::ArbitraryIPAddress;

    ActiveParticipant involvedApplication(Process::ArbitraryProcessID, true);
    involvedApplication.addAETitle(DICOM::ArbitraryAETitle3);
    involvedApplication.userName = Process::ArbitraryProcessName;
    involvedApplication.networkAccessPointType = NetworkAccessPointType::IPAddress;
    involvedApplication.networkAccessPointId = Machine::ArbitraryIPAddress;

    ActiveParticipant involvedUser(User::ArbitraryUserID, true);
    involvedUser.setAlternativeUserId("MS\\doe");
    involvedUser.userName = User::ArbitraryUserName;
    involvedUser.networkAccessPointType = NetworkAccessPointType::TelephoneNumber;
    involvedUser.networkAccessPointId = User::ArbitraryTelephoneNumber;

    ApplicationActivity activity(ApplicationEvent::Stopped, Outcome::MajorFailure, applicationInfo);

    activity.addInvolvedApplication(involvedApplication);
    activity.addInvolvedUser(involvedUser);

    auto nodes = activity.createNodes();

    ASSERT_THAT(nodes.size(), Eq(4));

    auto eventIdentification = nodes[0];
    ASSERT_THAT(eventIdentification.name(), Eq("EventIdentification"));
    checkEventIdentification(eventIdentification, "E", Outcome::MajorFailure);
    ASSERT_THAT(eventIdentification.nodes().size(), Eq(2));

    auto eventId = eventIdentification.nodes().at(0);
    ASSERT_THAT(eventId.name(), Eq("EventID"));
    checkCodedValueType(eventId, "110100", "Application Activity");

    auto eventTypeCode = eventIdentification.nodes().at(1);
    ASSERT_THAT(eventTypeCode.name(), Eq("EventTypeCode"));
    checkCodedValueType(eventTypeCode, "110121", "Application Stop");

    auto node = nodes[1];
    ASSERT_THAT(node.name(), Eq("ActiveParticipant"));
    checkActiveParticipantApplication(node);

    node = nodes[2];
    ASSERT_THAT(node.name(), Eq("ActiveParticipant"));
    checkActiveParticipantInvolvedApplication(node);
}

void ApplicationActivityTests::checkActiveParticipantApplication(const Node& activeParticipant)
{
    ASSERT_THAT(activeParticipant.attributes().size(), Eq(6));

    auto attribute = activeParticipant.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("UserID"));
    EXPECT_THAT(attribute.value, Eq(Process::ArbitraryProcessID));

    attribute = activeParticipant.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("AlternativeUserID"));
    EXPECT_THAT(attribute.value,
                Eq("AETITLES= " + DICOM::ArbitraryAETitle + ";" + DICOM::ArbitraryAETitle2));

    attribute = activeParticipant.attributes().at(2);
    ASSERT_THAT(attribute.name, Eq("UserName"));
    EXPECT_THAT(attribute.value, Eq(Process::ArbitraryProcessName));

    attribute = activeParticipant.attributes().at(3);
    ASSERT_THAT(attribute.name, Eq("UserIsRequestor"));
    EXPECT_THAT(attribute.value, Eq("false"));

    attribute = activeParticipant.attributes().at(4);
    ASSERT_THAT(attribute.name, Eq("NetworkAccessPointTypeCode"));
    EXPECT_THAT(attribute.value, Eq("2"));

    attribute = activeParticipant.attributes().at(5);
    ASSERT_THAT(attribute.name, Eq("NetworkAccessPointID"));
    EXPECT_THAT(attribute.value, Eq(Machine::ArbitraryIPAddress));

    ASSERT_THAT(activeParticipant.nodes().size(), Eq(1));

    auto node = activeParticipant.nodes().at(0);
    ASSERT_THAT(node.name(), Eq("RoleIDCode"));
    checkCodedValueType(node, "110150", "Application");
}

void ApplicationActivityTests::checkActiveParticipantInvolvedApplication(
    const Node& activeParticipant)
{
    ASSERT_THAT(activeParticipant.attributes().size(), Eq(6));

    auto attribute = activeParticipant.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("UserID"));
    EXPECT_THAT(attribute.value, Eq(Process::ArbitraryProcessID));

    attribute = activeParticipant.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("AlternativeUserID"));
    EXPECT_THAT(attribute.value, Eq("AETITLES= " + DICOM::ArbitraryAETitle3));

    attribute = activeParticipant.attributes().at(2);
    ASSERT_THAT(attribute.name, Eq("UserName"));
    EXPECT_THAT(attribute.value, Eq(Process::ArbitraryProcessName));

    attribute = activeParticipant.attributes().at(3);
    ASSERT_THAT(attribute.name, Eq("UserIsRequestor"));
    EXPECT_THAT(attribute.value, Eq("true"));

    attribute = activeParticipant.attributes().at(4);
    ASSERT_THAT(attribute.name, Eq("NetworkAccessPointTypeCode"));
    EXPECT_THAT(attribute.value, Eq("2"));

    attribute = activeParticipant.attributes().at(5);
    ASSERT_THAT(attribute.name, Eq("NetworkAccessPointID"));
    EXPECT_THAT(attribute.value, Eq(Machine::ArbitraryIPAddress));

    ASSERT_THAT(activeParticipant.nodes().size(), Eq(1));

    auto node = activeParticipant.nodes().at(0);
    ASSERT_THAT(node.name(), Eq("RoleIDCode"));
    checkCodedValueType(node, "110151", "Application Launcher");
}

void ApplicationActivityTests::checkActiveParticipantInvolvedUser(const Node& activeParticipant)
{
    ASSERT_THAT(activeParticipant.attributes().size(), Eq(6));

    auto attribute = activeParticipant.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("UserID"));
    EXPECT_THAT(attribute.value, Eq(User::ArbitraryUserID));

    attribute = activeParticipant.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("AlternativeUserID"));
    EXPECT_THAT(attribute.value, Eq("MS\\doe"));

    attribute = activeParticipant.attributes().at(2);
    ASSERT_THAT(attribute.name, Eq("UserName"));
    EXPECT_THAT(attribute.value, Eq(User::ArbitraryUserName));

    attribute = activeParticipant.attributes().at(3);
    ASSERT_THAT(attribute.name, Eq("UserIsRequestor"));
    EXPECT_THAT(attribute.value, Eq("true"));

    attribute = activeParticipant.attributes().at(4);
    ASSERT_THAT(attribute.name, Eq("NetworkAccessPointTypeCode"));
    EXPECT_THAT(attribute.value, Eq("3"));

    attribute = activeParticipant.attributes().at(5);
    ASSERT_THAT(attribute.name, Eq("NetworkAccessPointID"));
    EXPECT_THAT(attribute.value, Eq(User::ArbitraryTelephoneNumber));

    ASSERT_THAT(activeParticipant.nodes().size(), Eq(1));

    auto node = activeParticipant.nodes().at(0);
    ASSERT_THAT(node.name(), Eq("RoleIDCode"));
    checkCodedValueType(node, "110151", "Application Launcher");
}
