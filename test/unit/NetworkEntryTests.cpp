#include <gmock/gmock.h>

#include "NetworkEntry.h"
#include "CodedValueTypeTests.h"
#include "TestData.h"

using namespace AuditTrail;
using namespace IO;
using namespace Tests;
using namespace testing;

class NetworkEntryTests : public Test
{
public:
    void checkEnteringNode(const Node& enteringNode);
};

TEST_F(NetworkEntryTests, createNodes_WithAllAttributes_ReturnsCorrectNodes)
{
    NetworkEntry networkEntry(Outcome::Success, NetworkEntry::Action::Attach,
                              ActiveParticipant{User::ArbitraryUserID, false});

    auto nodes = networkEntry.createNodes();

    ASSERT_THAT(nodes.size(), Eq(2));

    auto eventIdentification = nodes[0];
    ASSERT_THAT(eventIdentification.name(), Eq("EventIdentification"));
    checkEventIdentification(eventIdentification, "E", Outcome::Success);

    ASSERT_THAT(eventIdentification.nodes().size(), Eq(2));

    auto eventId = eventIdentification.nodes().at(0);
    ASSERT_THAT(eventId.name(), Eq("EventID"));
    checkCodedValueType(eventId, "110108", "Network Entry");

    auto eventTypeCode = eventIdentification.nodes().at(1);
    ASSERT_THAT(eventTypeCode.name(), Eq("EventTypeCode"));
    checkCodedValueType(eventTypeCode, "110124", "Attach");

    auto node = nodes[1];
    ASSERT_THAT(node.name(), Eq("ActiveParticipant"));
    checkEnteringNode(node);
}

void NetworkEntryTests::checkEnteringNode(const Node& enteringNode)
{
    ASSERT_THAT(enteringNode.attributes().size(), Eq(2));

    auto attribute = enteringNode.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("UserID"));
    EXPECT_THAT(attribute.value, Eq(User::ArbitraryUserID));

    attribute = enteringNode.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("UserIsRequestor"));
    EXPECT_THAT(attribute.value, Eq("false"));

    ASSERT_THAT(enteringNode.nodes().size(), Eq(0));
}
