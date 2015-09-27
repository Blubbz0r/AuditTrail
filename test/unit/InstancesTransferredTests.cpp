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
};

TEST_F(InstancesTransferredTests, createNodes_WithAllAttributes_ReturnsCorrectNodes)
{
    InstancesTransferred instancesTransferred(Outcome::Success,
                                              InstancesTransferred::Action::Create,
                                              ActiveParticipant(Process::ArbitraryProcessID, true),
                                              ActiveParticipant(Process::ArbitraryProcessID, false));

    instancesTransferred.addOtherParticipant(ActiveParticipant(User::ArbitraryUserID, false));

    auto nodes = instancesTransferred.createNodes();

    ASSERT_THAT(nodes.size(), Eq(4));

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
