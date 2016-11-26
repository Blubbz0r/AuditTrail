#include "gmock/gmock.h"

#include "InstancesTransferred.h"
#include "CodedValueTypeTests.h"
#include "TestData.h"

using namespace AuditTrail;
using namespace IO;
using namespace Tests;
using namespace testing;

class InstancesTransferredTests : public Test
{
public:
    void checkSendingProcess(const Node& sendingProcess);

    void checkReceivingProcess(const Node& receivingProcess);

    void checkOtherParticipant(const Node& otherParticipant);

    void checkStudy(const Node& study);
    void checkDescription(const Node& description);

    void checkPatient(const Node& patient);
};

TEST_F(InstancesTransferredTests, createNodes_WithAllAttributes_ReturnsCorrectNodes)
{
    InstancesTransferred instancesTransferred(
        Outcome::Success, InstancesTransferred::Action::Create,
        ActiveParticipant(Process::ArbitraryProcessID, true),
        ActiveParticipant(Process::ArbitraryProcessID, false), DICOM::ArbitraryPatientID);

    instancesTransferred.addOtherParticipant(ActiveParticipant(User::ArbitraryUserID, false));

    std::vector<SOPClass> sopClasses;
    sopClasses.emplace_back(SOPClass{DICOM::ArbitrarySOPClassUID, DICOM::ArbitraryNumberOfInstances});
    instancesTransferred.addStudy(DICOM::ArbitraryStudyInstanceUID, sopClasses);

    instancesTransferred.setPatientName(DICOM::ArbitraryPatientName);

    auto nodes = instancesTransferred.createNodes();

    ASSERT_THAT(nodes.size(), Eq(6));

    auto eventIdentification = nodes[0];
    ASSERT_THAT(eventIdentification.name(), Eq("EventIdentification"));
    checkEventIdentification(eventIdentification, "C", Outcome::Success);

    ASSERT_THAT(eventIdentification.nodes().size(), Eq(1));

    auto eventId = eventIdentification.nodes().at(0);
    ASSERT_THAT(eventId.name(), Eq("EventID"));
    checkCodedValueType(eventId, "110104", "DICOM Instances Transferred");

    auto node = nodes[1];
    ASSERT_THAT(node.name(), Eq("ActiveParticipant"));
    checkSendingProcess(node);

    node = nodes[2];
    ASSERT_THAT(node.name(), Eq("ActiveParticipant"));
    checkReceivingProcess(node);

    node = nodes[3];
    ASSERT_THAT(node.name(), Eq("ActiveParticipant"));
    checkOtherParticipant(node);

    node = nodes[4];
    ASSERT_THAT(node.name(), Eq("ParticipantObjectIdentification"));
    checkStudy(node);

    node = nodes[5];
    ASSERT_THAT(node.name(), Eq("ParticipantObjectIdentification"));
    checkPatient(node);
}

void InstancesTransferredTests::checkSendingProcess(const Node& sendingProcess)
{
    ASSERT_THAT(sendingProcess.attributes().size(), Eq(2));

    auto attribute = sendingProcess.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("UserID"));
    EXPECT_THAT(attribute.value, Eq(Process::ArbitraryProcessID));

    attribute = sendingProcess.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("UserIsRequestor"));
    EXPECT_THAT(attribute.value, Eq("true"));

    ASSERT_THAT(sendingProcess.nodes().size(), Eq(1));

    auto node = sendingProcess.nodes().at(0);
    ASSERT_THAT(node.name(), Eq("RoleIDCode"));
    checkCodedValueType(node, "110153", "Source Role ID");
}

void InstancesTransferredTests::checkReceivingProcess(const Node& receivingProcess)
{
    ASSERT_THAT(receivingProcess.attributes().size(), Eq(2));

    auto attribute = receivingProcess.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("UserID"));
    EXPECT_THAT(attribute.value, Eq(Process::ArbitraryProcessID));

    attribute = receivingProcess.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("UserIsRequestor"));
    EXPECT_THAT(attribute.value, Eq("false"));

    ASSERT_THAT(receivingProcess.nodes().size(), Eq(1));

    auto node = receivingProcess.nodes().at(0);
    ASSERT_THAT(node.name(), Eq("RoleIDCode"));
    checkCodedValueType(node, "110152", "Destination Role ID");
}

void InstancesTransferredTests::checkOtherParticipant(const Node& otherParticipant)
{
    ASSERT_THAT(otherParticipant.attributes().size(), Eq(2));

    auto attribute = otherParticipant.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("UserID"));
    EXPECT_THAT(attribute.value, Eq(User::ArbitraryUserID));

    attribute = otherParticipant.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("UserIsRequestor"));
    EXPECT_THAT(attribute.value, Eq("false"));

    ASSERT_THAT(otherParticipant.nodes().size(), Eq(0));
}

void InstancesTransferredTests::checkStudy(const Node& study)
{
    ASSERT_THAT(study.attributes().size(), Eq(3));

    auto attribute = study.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("ParticipantObjectID"));
    EXPECT_THAT(attribute.value, Eq(DICOM::ArbitraryStudyInstanceUID));

    attribute = study.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("ParticipantObjectTypeCode"));
    EXPECT_THAT(attribute.value, Eq("2"));

    attribute = study.attributes().at(2);
    ASSERT_THAT(attribute.name, Eq("ParticipantObjectTypeCodeRole"));
    EXPECT_THAT(attribute.value, Eq("3"));

    ASSERT_THAT(study.nodes().size(), Eq(2));

    auto node = study.nodes().at(0);
    ASSERT_THAT(node.name(), Eq("ParticipantObjectIDTypeCode"));
    checkCodedValueType(node, "110180", "Study Instance UID");

    node = study.nodes().at(1);
    ASSERT_THAT(node.name(), Eq("ParticipantObjectDescription"));
    checkDescription(node);
}

void InstancesTransferredTests::checkDescription(const Node& description)
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

void InstancesTransferredTests::checkPatient(const Node& patient)
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
    checkCodedValueType(node, "2");

    node = patient.nodes().at(1);
    ASSERT_THAT(node.name(), Eq("ParticipantObjectName"));
    ASSERT_THAT(node.value(), Eq(DICOM::ArbitraryPatientName));
}
