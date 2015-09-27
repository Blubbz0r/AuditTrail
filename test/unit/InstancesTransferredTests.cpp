#include "gmock/gmock.h"

#include "InstancesTransferred.h"
#include "TestData.h"

using namespace AuditTrail;
using namespace IO;
using namespace Tests;
using namespace testing;

class InstancesTransferredTests : public Test
{
public:
    void checkEventIdentification(const Node& eventIdentification);
    void checkEventId(const Node& eventId);

    void checkSendingProcess(const Node& sendingProcess);
    void checkSendingProcessRoleIdCode(const Node& roleIdCode);

    void checkReceivingProcess(const Node& receivingProcess);
    void checkReceivingProcessRoleIdCode(const Node& roleIdCode);

    void checkOtherParticipant(const Node& otherParticipant);

    void checkStudy(const Node& study);
    void checkStudyIdTypeCode(const Node& idTypeCode);
    void checkDescription(const Node& description);
};

TEST_F(InstancesTransferredTests, createNodes_WithAllAttributes_ReturnsCorrectNodes)
{
    InstancesTransferred instancesTransferred(Outcome::Success,
                                              InstancesTransferred::Action::Create,
                                              ActiveParticipant(Process::ArbitraryProcessID, true),
                                              ActiveParticipant(Process::ArbitraryProcessID, false));

    instancesTransferred.addOtherParticipant(ActiveParticipant(User::ArbitraryUserID, false));

    std::vector<SOPClass> sopClasses;
    sopClasses.emplace_back(SOPClass{DICOM::ArbitrarySOPClassUID, DICOM::ArbitraryNumberOfInstances});
    instancesTransferred.addStudy(DICOM::ArbitraryStudyInstanceUID, sopClasses);

    auto nodes = instancesTransferred.createNodes();

    ASSERT_THAT(nodes.size(), Eq(5));

    auto node = nodes[0];
    ASSERT_THAT(node.name(), Eq("EventIdentification"));
    checkEventIdentification(node);

    node = nodes[1];
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
}

void InstancesTransferredTests::checkEventIdentification(const Node& eventIdentification)
{
    ASSERT_THAT(eventIdentification.attributes().size(), Eq(3));

    auto attribute = eventIdentification.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("EventActionCode"));
    EXPECT_THAT(attribute.value, Eq("C"));

    attribute = eventIdentification.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("EventDateTime"));
    // todo: how to check the date time?

    attribute = eventIdentification.attributes().at(2);
    ASSERT_THAT(attribute.name, Eq("EventOutcomeIndicator"));
    EXPECT_THAT(attribute.value, Eq(std::to_string(static_cast<int>(Outcome::Success))));

    ASSERT_THAT(eventIdentification.nodes().size(), Eq(1));

    auto node = eventIdentification.nodes().at(0);
    ASSERT_THAT(node.name(), Eq("EventID"));
    checkEventId(node);
}

void InstancesTransferredTests::checkEventId(const Node& eventId)
{
    ASSERT_THAT(eventId.attributes().size(), Eq(3));

    auto attribute = eventId.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("code"));
    EXPECT_THAT(attribute.value, Eq("110104"));

    attribute = eventId.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("codeSystemName"));
    EXPECT_THAT(attribute.value, Eq("DCM"));

    attribute = eventId.attributes().at(2);
    ASSERT_THAT(attribute.name, Eq("displayName"));
    EXPECT_THAT(attribute.value, Eq("DICOM Instances Transferred"));
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
    checkSendingProcessRoleIdCode(node);
}

void InstancesTransferredTests::checkSendingProcessRoleIdCode(const Node& roleIdCode)
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
    checkReceivingProcessRoleIdCode(node);
}

void InstancesTransferredTests::checkReceivingProcessRoleIdCode(const Node& roleIdCode)
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
    checkStudyIdTypeCode(node);

    node = study.nodes().at(1);
    ASSERT_THAT(node.name(), Eq("ParticipantObjectDescription"));
    checkDescription(node);
}

void InstancesTransferredTests::checkStudyIdTypeCode(const Node& idTypeCode)
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
