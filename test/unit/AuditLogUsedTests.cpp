#include "gmock/gmock.h"

#include "AuditLogUsed.h"
#include "TestData.h"

using namespace AuditTrail;
using namespace IO;
using namespace Tests;
using namespace testing;

class AuditLogUsedTests : public Test
{
public:
    void checkEventIdentification(const Node& eventIdentification);
    void checkEventId(const Node& eventId);

    void checkActiveParticipantApplication(const Node& activeParticipant);
    void checkActiveParticipantUser(const Node& activeParticipant);

    void checkParticipantObject(const Node& participantObject);
    void checkParticipantObjectIDTypeCode(const Node& idTypeCode);
    void checkParticipantObjectName(const Node& objectName);
};

TEST_F(AuditLogUsedTests, createNodes_WithAllAttributes_ReturnsCorrectNodes)
{
    AuditLogUsed auditLogUsed(Outcome::MinorFailure,
                              "file:///C:/ProgramData/iQ-ROBOT/AuditLog/log.xml");

    ActiveParticipant accessingApplication(Process::ArbitraryProcessID, true);
    accessingApplication.addAETitle(DICOM::ArbitraryAETitle);
    accessingApplication.userName = Process::ArbitraryProcessName;
    accessingApplication.networkAccessPointType = NetworkAccessPointType::MachineName;
    accessingApplication.networkAccessPointId = "CT001";
    auditLogUsed.setAccessingApplication(accessingApplication);

    ActiveParticipant accessingUser(User::ArbitraryUserID, true);
    accessingUser.setAlternativeUserId("MS\\doe");
    accessingUser.userName = User::ArbitraryUserName;
    accessingUser.networkAccessPointType = NetworkAccessPointType::TelephoneNumber;
    accessingUser.networkAccessPointId = User::ArbitraryTelephoneNumber;
    auditLogUsed.setAccessingUser(accessingUser);

    auto nodes = auditLogUsed.createNodes();

    ASSERT_THAT(nodes.size(), Eq(4));

    auto node = nodes[0];
    ASSERT_THAT(node.name(), Eq("EventIdentification"));
    checkEventIdentification(node);

    node = nodes[1];
    ASSERT_THAT(node.name(), Eq("ActiveParticipant"));
    checkActiveParticipantApplication(node);

    node = nodes[2];
    ASSERT_THAT(node.name(), Eq("ActiveParticipant"));
    checkActiveParticipantUser(node);

    node = nodes[3];
    ASSERT_THAT(node.name(), Eq("ParticipantObjectIdentification"));
    checkParticipantObject(node);
}

void AuditLogUsedTests::checkEventIdentification(const Node& eventIdentification)
{
    ASSERT_THAT(eventIdentification.attributes().size(), Eq(3));

    auto attribute = eventIdentification.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("EventActionCode"));
    EXPECT_THAT(attribute.value, Eq("R"));

    attribute = eventIdentification.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("EventDateTime"));
    // todo: how to check the date time?

    attribute = eventIdentification.attributes().at(2);
    ASSERT_THAT(attribute.name, Eq("EventOutcomeIndicator"));
    EXPECT_THAT(attribute.value, Eq(std::to_string(static_cast<int>(Outcome::MinorFailure))));

    ASSERT_THAT(eventIdentification.nodes().size(), Eq(1));

    auto node = eventIdentification.nodes().at(0);
    ASSERT_THAT(node.name(), Eq("EventID"));
    checkEventId(node);
}

void AuditLogUsedTests::checkEventId(const Node& eventId)
{
    ASSERT_THAT(eventId.attributes().size(), Eq(3));

    auto attribute = eventId.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("code"));
    EXPECT_THAT(attribute.value, Eq("110101"));

    attribute = eventId.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("codeSystemName"));
    EXPECT_THAT(attribute.value, Eq("DCM"));

    attribute = eventId.attributes().at(2);
    ASSERT_THAT(attribute.name, Eq("displayName"));
    EXPECT_THAT(attribute.value, Eq("Audit Log Used"));
}

void AuditLogUsedTests::checkActiveParticipantApplication(const Node& activeParticipant)
{
    ASSERT_THAT(activeParticipant.attributes().size(), Eq(6));

    auto attribute = activeParticipant.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("UserID"));
    EXPECT_THAT(attribute.value, Eq(Process::ArbitraryProcessID));

    attribute = activeParticipant.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("AlternativeUserID"));
    EXPECT_THAT(attribute.value, Eq("AETITLES= " + DICOM::ArbitraryAETitle));

    attribute = activeParticipant.attributes().at(2);
    ASSERT_THAT(attribute.name, Eq("UserName"));
    EXPECT_THAT(attribute.value, Eq(Process::ArbitraryProcessName));

    attribute = activeParticipant.attributes().at(3);
    ASSERT_THAT(attribute.name, Eq("UserIsRequestor"));
    EXPECT_THAT(attribute.value, Eq("true"));

    attribute = activeParticipant.attributes().at(4);
    ASSERT_THAT(attribute.name, Eq("NetworkAccessPointTypeCode"));
    EXPECT_THAT(attribute.value, Eq("1"));

    attribute = activeParticipant.attributes().at(5);
    ASSERT_THAT(attribute.name, Eq("NetworkAccessPointID"));
    EXPECT_THAT(attribute.value, Eq("CT001"));
}

void AuditLogUsedTests::checkActiveParticipantUser(const Node& activeParticipant)
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
}

void AuditLogUsedTests::checkParticipantObject(const Node& participantObject)
{
    ASSERT_THAT(participantObject.attributes().size(), Eq(3));

    auto attribute = participantObject.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("ParticipantObjectID"));
    EXPECT_THAT(attribute.value, Eq("file:///C:/ProgramData/iQ-ROBOT/AuditLog/log.xml"));

    attribute = participantObject.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("ParticipantObjectTypeCode"));
    EXPECT_THAT(attribute.value, Eq("2"));

    attribute = participantObject.attributes().at(2);
    ASSERT_THAT(attribute.name, Eq("ParticipantObjectTypeCodeRole"));
    EXPECT_THAT(attribute.value, Eq("13"));

    ASSERT_THAT(participantObject.nodes().size(), Eq(2));

    auto node = participantObject.nodes().at(0);
    ASSERT_THAT(node.name(), Eq("ParticipantObjectIDTypeCode"));
    checkParticipantObjectIDTypeCode(node);

    node = participantObject.nodes().at(1);
    ASSERT_THAT(node.name(), Eq("ParticipantObjectName"));
    checkParticipantObjectName(node);
}

void AuditLogUsedTests::checkParticipantObjectIDTypeCode(const Node& idTypeCode)
{
    ASSERT_THAT(idTypeCode.attributes().size(), Eq(1));

    auto attribute = idTypeCode.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("code"));
    EXPECT_THAT(attribute.value, Eq("12"));
}

void AuditLogUsedTests::checkParticipantObjectName(const Node& objectName)
{
    ASSERT_THAT(objectName.attributes().size(), Eq(0));
    EXPECT_THAT(objectName.value(), Eq("Security Audit Log"));
}
