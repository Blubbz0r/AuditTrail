#include <gmock/gmock.h>

#include "Query.h"
#include "CodedValueTypeTests.h"
#include "TestData.h"

using namespace AuditTrail;
using namespace IO;
using namespace Tests;
using namespace testing;

class QueryTests : public Test
{
public:
    void checkQueryingProcess(const Node& queryingProcess);

    void checkQueriedProcess(const Node& queriedProcess);

    void checkOtherProcess(const Node& otherProcess);
};

TEST_F(QueryTests, createNodes_WithAllAtrributes_ReturnsCorrectNodes)
{
    Query query(Outcome::Success, ActiveParticipant{User::ArbitraryUserID, true},
                ActiveParticipant{Process::ArbitraryProcessID, false}, DICOM::ArbitrarySOPClassUID,
                "BASE64_ENCODED_DATASET" /* TODO: do we want to provide Base64? */);

    query.addOtherParticipant(ActiveParticipant{ "OtherProcess", false });

    auto nodes = query.createNodes();

    ASSERT_THAT(nodes.size(), Eq(4));

    auto eventIdentification = nodes[0];
    ASSERT_THAT(eventIdentification.name(), Eq("EventIdentification"));
    checkEventIdentification(eventIdentification, "E", Outcome::Success);

    ASSERT_THAT(eventIdentification.nodes().size(), Eq(1));

    auto eventId = eventIdentification.nodes().at(0);
    ASSERT_THAT(eventId.name(), Eq("EventID"));
    checkCodedValueType(eventId, "110112", "Query");

    auto node = nodes[1];
    ASSERT_THAT(node.name(), Eq("ActiveParticipant"));
    checkQueryingProcess(node);

    node = nodes[2];
    ASSERT_THAT(node.name(), Eq("ActiveParticipant"));
    checkQueriedProcess(node);

    node = nodes[3];
    ASSERT_THAT(node.name(), Eq("ActiveParticipant"));
    checkOtherProcess(node);
}

void QueryTests::checkQueryingProcess(const Node& queryingProcess)
{
    ASSERT_THAT(queryingProcess.attributes().size(), Eq(2));

    auto attribute = queryingProcess.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("UserID"));
    EXPECT_THAT(attribute.value, Eq(User::ArbitraryUserID));

    attribute = queryingProcess.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("UserIsRequestor"));
    EXPECT_THAT(attribute.value, Eq("true"));

    ASSERT_THAT(queryingProcess.nodes().size(), Eq(1));
    auto node = queryingProcess.nodes().at(0);
    checkCodedValueType(node, "110153", "Source Role ID");
}

void QueryTests::checkQueriedProcess(const Node& queriedProcess)
{
    ASSERT_THAT(queriedProcess.attributes().size(), Eq(2));

    auto attribute = queriedProcess.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("UserID"));
    EXPECT_THAT(attribute.value, Eq(Process::ArbitraryProcessID));

    attribute = queriedProcess.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("UserIsRequestor"));
    EXPECT_THAT(attribute.value, Eq("false"));

    ASSERT_THAT(queriedProcess.nodes().size(), Eq(1));
    auto node = queriedProcess.nodes().at(0);
    checkCodedValueType(node, "110152", "Destination Role ID");
}

void QueryTests::checkOtherProcess(const Node& otherProcess)
{
    ASSERT_THAT(otherProcess.attributes().size(), Eq(2));

    auto attribute = otherProcess.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("UserID"));
    EXPECT_THAT(attribute.value, Eq("OtherProcess"));

    attribute = otherProcess.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("UserIsRequestor"));
    EXPECT_THAT(attribute.value, Eq("false"));
}
