#include <gmock/gmock.h>

#include "Query.h"
#include "TestData.h"

using namespace AuditTrail;
using namespace IO;
using namespace Tests;
using namespace testing;

class QueryTests : public Test
{
public:
    void checkEventIdentification(const Node& eventIdentification);
    void checkEventId(const Node& eventId);

    void checkQueryingProcess(const Node& queryingProcess);
    void checkRoleCodeId(const Node& roleCodeId, const std::string& code,
                         const std::string& displayName);

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

    auto node = nodes[0];
    ASSERT_THAT(node.name(), Eq("EventIdentification"));
    checkEventIdentification(node);

    node = nodes[1];
    ASSERT_THAT(node.name(), Eq("ActiveParticipant"));
    checkQueryingProcess(node);

    node = nodes[2];
    ASSERT_THAT(node.name(), Eq("ActiveParticipant"));
    checkQueriedProcess(node);

    node = nodes[3];
    ASSERT_THAT(node.name(), Eq("ActiveParticipant"));
    checkOtherProcess(node);
}

void QueryTests::checkEventIdentification(const Node& eventIdentification)
{
    ASSERT_THAT(eventIdentification.attributes().size(), Eq(3));

    auto attribute = eventIdentification.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("EventActionCode"));
    EXPECT_THAT(attribute.value, Eq("E"));

    attribute = eventIdentification.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("EventDateTime"));
    // TODO: how to check the date time?

    attribute = eventIdentification.attributes().at(2);
    ASSERT_THAT(attribute.name, Eq("EventOutcomeIndicator"));
    EXPECT_THAT(attribute.value, Eq(std::to_string(static_cast<int>(Outcome::Success))));

    ASSERT_THAT(eventIdentification.nodes().size(), Eq(1));

    auto node = eventIdentification.nodes().at(0);
    ASSERT_THAT(node.name(), Eq("EventID"));
    checkEventId(node);
}

void QueryTests::checkEventId(const Node& eventId)
{
    ASSERT_THAT(eventId.attributes().size(), Eq(3));

    auto attribute = eventId.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("code"));
    EXPECT_THAT(attribute.value, Eq("110112"));

    attribute = eventId.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("codeSystemName"));
    EXPECT_THAT(attribute.value, Eq("DCM"));

    attribute = eventId.attributes().at(2);
    ASSERT_THAT(attribute.name, Eq("displayName"));
    EXPECT_THAT(attribute.value, Eq("Query"));
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
    checkRoleCodeId(node, "110153", "Source Role ID");
}

void QueryTests::checkRoleCodeId(const Node& roleCodeId, const std::string& code,
                                 const std::string& displayName)
{
    ASSERT_THAT(roleCodeId.attributes().size(), Eq(3));

    auto attribute = roleCodeId.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("code"));
    EXPECT_THAT(attribute.value, Eq(code));

    attribute = roleCodeId.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("codeSystemName"));
    EXPECT_THAT(attribute.value, Eq("DCM"));

    attribute = roleCodeId.attributes().at(2);
    ASSERT_THAT(attribute.name, Eq("displayName"));
    EXPECT_THAT(attribute.value, Eq(displayName));
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
    checkRoleCodeId(node, "110152", "Destination Role ID");
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
