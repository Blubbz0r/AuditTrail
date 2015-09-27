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
};

TEST_F(InstancesTransferredTests, createNodes_WithAllAttributes_ReturnsCorrectNodes)
{
    InstancesTransferred instancesTransferred(Outcome::Success,
                                              InstancesTransferred::Action::Create);

    auto nodes = instancesTransferred.createNodes();

    ASSERT_THAT(nodes.size(), Eq(1));

    auto node = nodes[0];
    ASSERT_THAT(node.name(), Eq("EventIdentification"));
    checkEventIdentification(node);
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
