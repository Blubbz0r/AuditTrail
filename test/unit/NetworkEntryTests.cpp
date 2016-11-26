#include <gmock/gmock.h>

#include "NetworkEntry.h"
#include "TestData.h"

using namespace AuditTrail;
using namespace IO;
using namespace Tests;
using namespace testing;

class NetworkEntryTests : public Test
{
public:
    void checkEventIdentification(const Node& eventIdentification);
    void checkEventId(const Node& eventId);
    void checkEventTypeCode(const Node& eventTypeCode);

    void checkEnteringNode(const Node& enteringNode);
};

TEST_F(NetworkEntryTests, createNodes_WithAllAttributes_ReturnsCorrectNodes)
{
    NetworkEntry networkEntry(Outcome::Success, NetworkEntry::Action::Attach,
                              ActiveParticipant{User::ArbitraryUserID, false});

    auto nodes = networkEntry.createNodes();

    ASSERT_THAT(nodes.size(), Eq(2));

    auto node = nodes[0];
    ASSERT_THAT(node.name(), Eq("EventIdentification"));
    checkEventIdentification(node);

    node = nodes[1];
    ASSERT_THAT(node.name(), Eq("ActiveParticipant"));
    checkEnteringNode(node);
}

void NetworkEntryTests::checkEventIdentification(const Node& eventIdentification)
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
    EXPECT_THAT(attribute.value, Eq(std::to_string(static_cast<int>(Outcome::Success))));

    ASSERT_THAT(eventIdentification.nodes().size(), Eq(2));

    auto node = eventIdentification.nodes().at(0);
    ASSERT_THAT(node.name(), Eq("EventID"));
    checkEventId(node);

    node = eventIdentification.nodes().at(1);
    ASSERT_THAT(node.name(), Eq("EventTypeCode"));
    checkEventTypeCode(node);
}

void NetworkEntryTests::checkEventId(const Node& eventId)
{
    ASSERT_THAT(eventId.attributes().size(), Eq(3));

    auto attribute = eventId.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("code"));
    EXPECT_THAT(attribute.value, Eq("110108"));

    attribute = eventId.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("codeSystemName"));
    EXPECT_THAT(attribute.value, Eq("DCM"));

    attribute = eventId.attributes().at(2);
    ASSERT_THAT(attribute.name, Eq("displayName"));
    EXPECT_THAT(attribute.value, Eq("Network Entry"));
}

void NetworkEntryTests::checkEventTypeCode(const Node& eventTypeCode)
{
    ASSERT_THAT(eventTypeCode.attributes().size(), Eq(3));

    auto attribute = eventTypeCode.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("code"));
    EXPECT_THAT(attribute.value, Eq("110124"));

    attribute = eventTypeCode.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("codeSystemName"));
    EXPECT_THAT(attribute.value, Eq("DCM"));

    attribute = eventTypeCode.attributes().at(2);
    ASSERT_THAT(attribute.name, Eq("displayName"));
    EXPECT_THAT(attribute.value, Eq("Attach"));
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
