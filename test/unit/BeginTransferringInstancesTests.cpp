#include "gmock/gmock.h"

#include "BeginTransferringInstances.h"
#include "TestData.h"

using namespace AuditTrail;
using namespace IO;
using namespace Tests;
using namespace testing;

class BeginTransferringInstancesTests : public Test
{
public:
    void checkEventIdentification(const Node& eventIdentification);
    void checkEventId(const Node& eventId);

    void checkActiveParticipantProcessSendingData(const Node& activeParticipant);
    void checkSourceRoleIdCode(const Node& roleIdCode);

    void checkActiveParticipantProcessReceivingData(const Node& activeParticipant);
    void checkDestinationRoleIdCode(const Node& roleIdCode);

    void checkOtherParticipant(const Node& participant);

    void checkStudiesBeingTransferred(const Node& studies);
    void checkStudiesIdTypeCode(const Node& idTypeCode);
    void checkDescription(const Node& description);

    void checkPatient(const Node& patient);
    void checkPatientIDTypeCode(const Node& idTypeCode);
};

TEST_F(BeginTransferringInstancesTests, createNodes_WithAllAttributes_ReturnsCorrectNodes)
{
    BeginTransferringInstances beginTransferringInstances(
        Outcome::SeriousFailure, ActiveParticipant(Process::ArbitraryProcessID, true),
        ActiveParticipant(Process::ArbitraryProcessID, false), DICOM::ArbitraryPatientID);

    ActiveParticipant otherParticipant(User::ArbitraryUserID, false);
    beginTransferringInstances.addOtherParticipant(otherParticipant);

    std::vector<SOPClass> sopClasses;
    sopClasses.emplace_back(SOPClass{DICOM::ArbitrarySOPClassUID, DICOM::ArbitraryNumberOfInstances});
    beginTransferringInstances.addStudy(DICOM::ArbitraryStudyInstanceUID, sopClasses);

    beginTransferringInstances.setPatientName(DICOM::ArbitraryPatientName);

    auto nodes = beginTransferringInstances.createNodes();

    ASSERT_THAT(nodes.size(), Eq(6));

    auto node = nodes[0];
    ASSERT_THAT(node.name(), Eq("EventIdentification"));
    checkEventIdentification(node);

    node = nodes[1];
    ASSERT_THAT(node.name(), Eq("ActiveParticipant"));
    checkActiveParticipantProcessSendingData(node);

    node = nodes[2];
    ASSERT_THAT(node.name(), Eq("ActiveParticipant"));
    checkActiveParticipantProcessReceivingData(node);

    node = nodes[3];
    ASSERT_THAT(node.name(), Eq("ActiveParticipant"));
    checkOtherParticipant(node);

    node = nodes[4];
    ASSERT_THAT(node.name(), Eq("ParticipantObjectIdentification"));
    checkStudiesBeingTransferred(node);

    node = nodes[5];
    ASSERT_THAT(node.name(), Eq("ParticipantObjectIdentification"));
    checkPatient(node);
}

void BeginTransferringInstancesTests::checkEventIdentification(const Node& eventIdentification)
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
    EXPECT_THAT(attribute.value, Eq(std::to_string(static_cast<int>(Outcome::SeriousFailure))));

    ASSERT_THAT(eventIdentification.nodes().size(), Eq(1));

    auto node = eventIdentification.nodes().at(0);
    ASSERT_THAT(node.name(), Eq("EventID"));
    checkEventId(node);
}

void BeginTransferringInstancesTests::checkEventId(const Node& eventId)
{
    ASSERT_THAT(eventId.attributes().size(), Eq(3));

    auto attribute = eventId.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("code"));
    EXPECT_THAT(attribute.value, Eq("110102"));

    attribute = eventId.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("codeSystemName"));
    EXPECT_THAT(attribute.value, Eq("DCM"));

    attribute = eventId.attributes().at(2);
    ASSERT_THAT(attribute.name, Eq("displayName"));
    EXPECT_THAT(attribute.value, Eq("Begin Transferring DICOM Instances"));
}

void BeginTransferringInstancesTests::checkActiveParticipantProcessSendingData(
    const Node& activeParticipant)
{
    ASSERT_THAT(activeParticipant.attributes().size(), Eq(2));

    auto attribute = activeParticipant.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("UserID"));
    EXPECT_THAT(attribute.value, Eq(Process::ArbitraryProcessID));

    attribute = activeParticipant.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("UserIsRequestor"));
    EXPECT_THAT(attribute.value, Eq("true"));

    ASSERT_THAT(activeParticipant.nodes().size(), Eq(1));

    auto node = activeParticipant.nodes().at(0);
    ASSERT_THAT(node.name(), Eq("RoleIDCode"));
    checkSourceRoleIdCode(node);
}

void BeginTransferringInstancesTests::checkSourceRoleIdCode(const Node& roleIdCode)
{
    ASSERT_THAT(roleIdCode.attributes().size(), Eq(3));

    auto attribute = roleIdCode.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("code"));
    EXPECT_THAT(attribute.value, Eq("110153"));

    attribute = roleIdCode.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("codeSystemName"));
    EXPECT_THAT(attribute.value, Eq("DCM"));

    attribute = roleIdCode.attributes().at(2);
    ASSERT_THAT(attribute.name, Eq("displayName"));
    EXPECT_THAT(attribute.value, Eq("Source Role ID"));
}

void BeginTransferringInstancesTests::checkActiveParticipantProcessReceivingData(
    const Node& activeParticipant)
{
    ASSERT_THAT(activeParticipant.attributes().size(), Eq(2));

    auto attribute = activeParticipant.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("UserID"));
    EXPECT_THAT(attribute.value, Eq(Process::ArbitraryProcessID));

    attribute = activeParticipant.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("UserIsRequestor"));
    EXPECT_THAT(attribute.value, Eq("false"));

    ASSERT_THAT(activeParticipant.nodes().size(), Eq(1));

    auto node = activeParticipant.nodes().at(0);
    ASSERT_THAT(node.name(), Eq("RoleIDCode"));
    checkDestinationRoleIdCode(node);
}

void BeginTransferringInstancesTests::checkDestinationRoleIdCode(const Node& roleIdCode)
{
    ASSERT_THAT(roleIdCode.attributes().size(), Eq(3));

    auto attribute = roleIdCode.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("code"));
    EXPECT_THAT(attribute.value, Eq("110152"));

    attribute = roleIdCode.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("codeSystemName"));
    EXPECT_THAT(attribute.value, Eq("DCM"));

    attribute = roleIdCode.attributes().at(2);
    ASSERT_THAT(attribute.name, Eq("displayName"));
    EXPECT_THAT(attribute.value, Eq("Destination Role ID"));
}

void BeginTransferringInstancesTests::checkOtherParticipant(const Node& participant)
{
    ASSERT_THAT(participant.attributes().size(), Eq(2));

    auto attribute = participant.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("UserID"));
    EXPECT_THAT(attribute.value, Eq(User::ArbitraryUserID));

    attribute = participant.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("UserIsRequestor"));
    EXPECT_THAT(attribute.value, Eq("false"));
}

void BeginTransferringInstancesTests::checkStudiesBeingTransferred(const Node& studies)
{
    ASSERT_THAT(studies.attributes().size(), Eq(3));

    auto attribute = studies.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("ParticipantObjectID"));
    EXPECT_THAT(attribute.value, Eq(DICOM::ArbitraryStudyInstanceUID));

    attribute = studies.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("ParticipantObjectTypeCode"));
    EXPECT_THAT(attribute.value, Eq("2"));

    attribute = studies.attributes().at(2);
    ASSERT_THAT(attribute.name, Eq("ParticipantObjectTypeCodeRole"));
    EXPECT_THAT(attribute.value, Eq("3"));

    ASSERT_THAT(studies.nodes().size(), Eq(2));
    auto node = studies.nodes().at(0);
    ASSERT_THAT(node.name(), Eq("ParticipantObjectIDTypeCode"));
    checkStudiesIdTypeCode(node);

    node = studies.nodes().at(1);
    ASSERT_THAT(node.name(), Eq("ParticipantObjectDescription"));
    checkDescription(node);
}

void BeginTransferringInstancesTests::checkStudiesIdTypeCode(const Node& idTypeCode)
{
    ASSERT_THAT(idTypeCode.attributes().size(), Eq(3));

    auto attribute = idTypeCode.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("code"));
    EXPECT_THAT(attribute.value, Eq("110180"));

    attribute = idTypeCode.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("codeSystemName"));
    EXPECT_THAT(attribute.value, Eq("DCM"));

    attribute = idTypeCode.attributes().at(2);
    ASSERT_THAT(attribute.name, Eq("displayName"));
    EXPECT_THAT(attribute.value, Eq("Study Instance UID"));
}

void BeginTransferringInstancesTests::checkDescription(const Node& description)
{
    ASSERT_THAT(description.attributes().size(), Eq(0));

    ASSERT_THAT(description.nodes().size(), Eq(1));

    auto node = description.nodes().at(0);
    ASSERT_THAT(node.name(), Eq("SOPClass"));

    ASSERT_THAT(node.attributes().size(), Eq(2));
    auto attribute = node.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("UID"));
    EXPECT_THAT(attribute.value, Eq(DICOM::ArbitrarySOPClassUID));

    attribute = node.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("NumberOfInstances"));
    EXPECT_THAT(attribute.value, Eq(std::to_string(DICOM::ArbitraryNumberOfInstances)));
}

void BeginTransferringInstancesTests::checkPatient(const Node& patient)
{
    ASSERT_THAT(patient.attributes().size(), Eq(3));

    auto attribute = patient.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("ParticipantObjectID"));
    EXPECT_THAT(attribute.value, Eq(DICOM::ArbitraryPatientID));

    attribute = patient.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("ParticipantObjectTypeCode"));
    EXPECT_THAT(attribute.value, Eq("1"));

    attribute = patient.attributes().at(2);
    ASSERT_THAT(attribute.name, Eq("ParticipantObjectTypeCodeRole"));
    EXPECT_THAT(attribute.value, Eq("1"));

    ASSERT_THAT(patient.nodes().size(), Eq(2));
    auto node = patient.nodes().at(0);
    ASSERT_THAT(node.name(), Eq("ParticipantObjectIDTypeCode"));
    checkPatientIDTypeCode(node);

    node = patient.nodes().at(1);
    ASSERT_THAT(node.name(), Eq("ParticipantObjectName"));
    ASSERT_THAT(node.value(), Eq(DICOM::ArbitraryPatientName));
}

void BeginTransferringInstancesTests::checkPatientIDTypeCode(const Node& idTypeCode)
{
    ASSERT_THAT(idTypeCode.attributes().size(), Eq(1));

    auto attribute = idTypeCode.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("code"));
    EXPECT_THAT(attribute.value, Eq("2"));
}
