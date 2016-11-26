#include <gmock/gmock.h>

#include "UserAuthentication.h"
#include "TestData.h"

using namespace AuditTrail;
using namespace IO;
using namespace Tests;
using namespace testing;

class UserAuthenticationTests : public Test
{
public:
    void checkEventIdentification(const Node& eventIdentification);
    void checkEventId(const Node& eventId);
    void checkEventTypeCode(const Node& eventTypeCode);

    void checkAuthenticatedPerson(const Node& authenticatedPerson);

    void checkAuthenticatingSystem(const Node& authenticatingSystem);
};

TEST_F(UserAuthenticationTests, createNodes_WithAllAttributes_ReturnsCorrectNodes)
{
    UserAuthentication userAuthentication(Outcome::Success, UserAuthentication::Type::Logout,
                                          ActiveParticipant{User::ArbitraryUserID, true});

    userAuthentication.setAuthenticatingSystem(
        ActiveParticipant{Process::ArbitraryProcessID, false});

    auto nodes = userAuthentication.createNodes();

    ASSERT_THAT(nodes.size(), Eq(3));

    auto node = nodes[0];
    ASSERT_THAT(node.name(), Eq("EventIdentification"));
    checkEventIdentification(node);

    node = nodes[1];
    ASSERT_THAT(node.name(), Eq("ActiveParticipant"));
    checkAuthenticatedPerson(node);

    node = nodes[2];
    ASSERT_THAT(node.name(), Eq("ActiveParticipant"));
    checkAuthenticatingSystem(node);
}

void UserAuthenticationTests::checkEventIdentification(const Node& eventIdentification)
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

void UserAuthenticationTests::checkEventId(const Node& eventId)
{
    ASSERT_THAT(eventId.attributes().size(), Eq(3));

    auto attribute = eventId.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("code"));
    EXPECT_THAT(attribute.value, Eq("110114"));

    attribute = eventId.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("codeSystemName"));
    EXPECT_THAT(attribute.value, Eq("DCM"));

    attribute = eventId.attributes().at(2);
    ASSERT_THAT(attribute.name, Eq("displayName"));
    EXPECT_THAT(attribute.value, Eq("User Authentication"));
}

void UserAuthenticationTests::checkEventTypeCode(const Node& eventTypeCode)
{
    ASSERT_THAT(eventTypeCode.attributes().size(), Eq(3));

    auto attribute = eventTypeCode.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("code"));
    EXPECT_THAT(attribute.value, Eq("110123"));

    attribute = eventTypeCode.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("codeSystemName"));
    EXPECT_THAT(attribute.value, Eq("DCM"));

    attribute = eventTypeCode.attributes().at(2);
    ASSERT_THAT(attribute.name, Eq("displayName"));
    EXPECT_THAT(attribute.value, Eq("Logout"));
}

void UserAuthenticationTests::checkAuthenticatedPerson(const Node& authenticatedPerson)
{
    ASSERT_THAT(authenticatedPerson.attributes().size(), Eq(2));

    auto attribute = authenticatedPerson.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("UserID"));
    EXPECT_THAT(attribute.value, Eq(User::ArbitraryUserID));

    attribute = authenticatedPerson.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("UserIsRequestor"));
    EXPECT_THAT(attribute.value, Eq("true"));

    ASSERT_THAT(authenticatedPerson.nodes().size(), Eq(0));
}

void UserAuthenticationTests::checkAuthenticatingSystem(const Node& authenticatingSystem)
{
    ASSERT_THAT(authenticatingSystem.attributes().size(), Eq(2));

    auto attribute = authenticatingSystem.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("UserID"));
    EXPECT_THAT(attribute.value, Eq(Process::ArbitraryProcessID));

    attribute = authenticatingSystem.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("UserIsRequestor"));
    EXPECT_THAT(attribute.value, Eq("false"));

    ASSERT_THAT(authenticatingSystem.nodes().size(), Eq(0));
}
