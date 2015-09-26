#include "gmock/gmock.h"

#include "ApplicationActivity.h"

using namespace AuditTrail;
using namespace IO;
using namespace testing;

class ApplicationActivityTests : public Test
{
public:
    void checkEventIdentification(const Node& eventIdentification);
    void checkEventId(const Node& eventId);
    void checkEventTypeCode(const Node& eventTypeCode);

    void checkActiveParticipantApplication(const Node& activeParticipant);
    void checkRoleIdCodeApplication(const Node& roleIdCode);

    void checkActiveParticipantInvolvedApplication(const Node& activeParticipant);
    void checkActiveParticipantInvolvedUser(const Node& activeParticipant);
    void checkRoleIdCodeInvolvedEntity(const Node& roleIdCode);
};

TEST_F(ApplicationActivityTests, createNodes_WithAllAttributes_ReturnsCorrectNodes)
{
    ActiveParticipant applicationInfo("123", false);
    applicationInfo.addAETitle("ROBOT1");
    applicationInfo.addAETitle("ROBOT2");
    applicationInfo.userName = "iQ-ROBOT";
    applicationInfo.networkAccessPointType = NetworkAccessPointType::IPAddress;
    applicationInfo.networkAccessPointId = "192.168.120.111";

    ActiveParticipant involvedApplication("234", true);
    involvedApplication.addAETitle("SERVER");
    involvedApplication.userName = "iQ-VIEW";
    involvedApplication.networkAccessPointType = NetworkAccessPointType::IPAddress;
    involvedApplication.networkAccessPointId = "192.168.120.123";

    ActiveParticipant involvedUser("habrich@image-systems.local", true);
    involvedUser.setAlternativeUserId("IIS\\habrich");
    involvedUser.userName = "Robert Habrich";
    involvedUser.networkAccessPointType = NetworkAccessPointType::TelephoneNumber;
    involvedUser.networkAccessPointId = "123456789";

    ApplicationActivity activity(ApplicationEvent::Stopped, Outcome::MajorFailure, applicationInfo);

    activity.addInvolvedApplication(involvedApplication);
    activity.addInvolvedUser(involvedUser);

    auto nodes = activity.createNodes();

    ASSERT_THAT(nodes.size(), Eq(4));

    auto node = nodes[0];
    ASSERT_THAT(node.name(), Eq("EventIdentification"));
    checkEventIdentification(node);

    node = nodes[1];
    ASSERT_THAT(node.name(), Eq("ActiveParticipant"));
    checkActiveParticipantApplication(node);

    node = nodes[2];
    ASSERT_THAT(node.name(), Eq("ActiveParticipant"));
    checkActiveParticipantInvolvedApplication(node);
}

void ApplicationActivityTests::checkEventIdentification(const Node& eventIdentification)
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
    EXPECT_THAT(attribute.value, Eq(std::to_string(static_cast<int>(Outcome::MajorFailure))));

    ASSERT_THAT(eventIdentification.nodes().size(), Eq(2));

    auto node = eventIdentification.nodes().at(0);
    ASSERT_THAT(node.name(), Eq("EventID"));
    checkEventId(node);

    node = eventIdentification.nodes().at(1);
    ASSERT_THAT(node.name(), Eq("EventTypeCode"));
    checkEventTypeCode(node);
}

void ApplicationActivityTests::checkEventId(const Node& eventId)
{
    ASSERT_THAT(eventId.attributes().size(), Eq(3));

    auto attribute = eventId.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("code"));
    EXPECT_THAT(attribute.value, Eq("110100"));

    attribute = eventId.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("codeSystemName"));
    EXPECT_THAT(attribute.value, Eq("DCM"));

    attribute = eventId.attributes().at(2);
    ASSERT_THAT(attribute.name, Eq("displayName"));
    EXPECT_THAT(attribute.value, Eq("Application Activity"));
}

void ApplicationActivityTests::checkEventTypeCode(const Node& eventTypeCode)
{
    ASSERT_THAT(eventTypeCode.attributes().size(), Eq(3));

    auto attribute = eventTypeCode.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("code"));
    EXPECT_THAT(attribute.value, Eq("110121"));

    attribute = eventTypeCode.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("codeSystemName"));
    EXPECT_THAT(attribute.value, Eq("DCM"));

    attribute = eventTypeCode.attributes().at(2);
    ASSERT_THAT(attribute.name, Eq("displayName"));
    EXPECT_THAT(attribute.value, Eq("Application Stop"));
}

void ApplicationActivityTests::checkActiveParticipantApplication(const Node& activeParticipant)
{
    ASSERT_THAT(activeParticipant.attributes().size(), Eq(6));

    auto attribute = activeParticipant.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("UserID"));
    EXPECT_THAT(attribute.value, Eq("123"));

    attribute = activeParticipant.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("AlternativeUserID"));
    EXPECT_THAT(attribute.value, Eq("AETITLES= ROBOT1;ROBOT2"));

    attribute = activeParticipant.attributes().at(2);
    ASSERT_THAT(attribute.name, Eq("UserName"));
    EXPECT_THAT(attribute.value, Eq("iQ-ROBOT"));

    attribute = activeParticipant.attributes().at(3);
    ASSERT_THAT(attribute.name, Eq("UserIsRequestor"));
    EXPECT_THAT(attribute.value, Eq("false"));

    attribute = activeParticipant.attributes().at(4);
    ASSERT_THAT(attribute.name, Eq("NetworkAccessPointTypeCode"));
    EXPECT_THAT(attribute.value, Eq("2"));

    attribute = activeParticipant.attributes().at(5);
    ASSERT_THAT(attribute.name, Eq("NetworkAccessPointID"));
    EXPECT_THAT(attribute.value, Eq("192.168.120.111"));

    ASSERT_THAT(activeParticipant.nodes().size(), Eq(1));

    auto node = activeParticipant.nodes().at(0);
    ASSERT_THAT(node.name(), Eq("RoleIDCode"));
    checkRoleIdCodeApplication(node);
}

void ApplicationActivityTests::checkRoleIdCodeApplication(const Node& roleIdCode)
{
    ASSERT_THAT(roleIdCode.attributes().size(), Eq(3));

    auto attribute = roleIdCode.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("code"));
    EXPECT_THAT(attribute.value, Eq("110150"));

    attribute = roleIdCode.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("codeSystemName"));
    EXPECT_THAT(attribute.value, Eq("DCM"));

    attribute = roleIdCode.attributes().at(2);
    ASSERT_THAT(attribute.name, Eq("displayName"));
    EXPECT_THAT(attribute.value, Eq("Application"));
}

void ApplicationActivityTests::checkActiveParticipantInvolvedApplication(
    const Node& activeParticipant)
{
    ASSERT_THAT(activeParticipant.attributes().size(), Eq(6));

    auto attribute = activeParticipant.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("UserID"));
    EXPECT_THAT(attribute.value, Eq("234"));

    attribute = activeParticipant.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("AlternativeUserID"));
    EXPECT_THAT(attribute.value, Eq("AETITLES= SERVER"));

    attribute = activeParticipant.attributes().at(2);
    ASSERT_THAT(attribute.name, Eq("UserName"));
    EXPECT_THAT(attribute.value, Eq("iQ-VIEW"));

    attribute = activeParticipant.attributes().at(3);
    ASSERT_THAT(attribute.name, Eq("UserIsRequestor"));
    EXPECT_THAT(attribute.value, Eq("true"));

    attribute = activeParticipant.attributes().at(4);
    ASSERT_THAT(attribute.name, Eq("NetworkAccessPointTypeCode"));
    EXPECT_THAT(attribute.value, Eq("2"));

    attribute = activeParticipant.attributes().at(5);
    ASSERT_THAT(attribute.name, Eq("NetworkAccessPointID"));
    EXPECT_THAT(attribute.value, Eq("192.168.120.123"));

    ASSERT_THAT(activeParticipant.nodes().size(), Eq(1));

    auto node = activeParticipant.nodes().at(0);
    ASSERT_THAT(node.name(), Eq("RoleIDCode"));
    checkRoleIdCodeInvolvedEntity(node);
}

void ApplicationActivityTests::checkActiveParticipantInvolvedUser(const Node& activeParticipant)
{
    ASSERT_THAT(activeParticipant.attributes().size(), Eq(6));

    auto attribute = activeParticipant.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("UserID"));
    EXPECT_THAT(attribute.value, Eq("habrich@image-systems.local"));

    attribute = activeParticipant.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("AlternativeUserID"));
    EXPECT_THAT(attribute.value, Eq("IIS\\habrich"));

    attribute = activeParticipant.attributes().at(2);
    ASSERT_THAT(attribute.name, Eq("UserName"));
    EXPECT_THAT(attribute.value, Eq("Robert Habrich"));

    attribute = activeParticipant.attributes().at(3);
    ASSERT_THAT(attribute.name, Eq("UserIsRequestor"));
    EXPECT_THAT(attribute.value, Eq("true"));

    attribute = activeParticipant.attributes().at(4);
    ASSERT_THAT(attribute.name, Eq("NetworkAccessPointTypeCode"));
    EXPECT_THAT(attribute.value, Eq("3"));

    attribute = activeParticipant.attributes().at(5);
    ASSERT_THAT(attribute.name, Eq("NetworkAccessPointID"));
    EXPECT_THAT(attribute.value, Eq("123456789"));

    ASSERT_THAT(activeParticipant.nodes().size(), Eq(1));

    auto node = activeParticipant.nodes().at(0);
    ASSERT_THAT(node.name(), Eq("RoleIDCode"));
    checkRoleIdCodeInvolvedEntity(node);
}

void ApplicationActivityTests::checkRoleIdCodeInvolvedEntity(const Node& roleIdCode)
{
    ASSERT_THAT(roleIdCode.attributes().size(), Eq(3));

    auto attribute = roleIdCode.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("code"));
    EXPECT_THAT(attribute.value, Eq("110151"));

    attribute = roleIdCode.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("codeSystemName"));
    EXPECT_THAT(attribute.value, Eq("DCM"));

    attribute = roleIdCode.attributes().at(2);
    ASSERT_THAT(attribute.name, Eq("displayName"));
    EXPECT_THAT(attribute.value, Eq("Application Launcher"));
}
