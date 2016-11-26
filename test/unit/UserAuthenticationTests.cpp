#include <gmock/gmock.h>

#include "UserAuthentication.h"
#include "CodedValueTypeTests.h"
#include "TestData.h"

using namespace AuditTrail;
using namespace IO;
using namespace Tests;
using namespace testing;

class UserAuthenticationTests : public Test
{
public:
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

    auto eventIdentification = nodes[0];
    ASSERT_THAT(eventIdentification.name(), Eq("EventIdentification"));
    checkEventIdentification(eventIdentification, "E", Outcome::Success);

    ASSERT_THAT(eventIdentification.nodes().size(), Eq(2));

    auto eventId = eventIdentification.nodes().at(0);
    ASSERT_THAT(eventId.name(), Eq("EventID"));
    checkCodedValueType(eventId, "110114", "User Authentication");

    auto eventTypeCode = eventIdentification.nodes().at(1);
    ASSERT_THAT(eventTypeCode.name(), Eq("EventTypeCode"));
    checkCodedValueType(eventTypeCode, "110123", "Logout");

    auto node = nodes[1];
    ASSERT_THAT(node.name(), Eq("ActiveParticipant"));
    checkAuthenticatedPerson(node);

    node = nodes[2];
    ASSERT_THAT(node.name(), Eq("ActiveParticipant"));
    checkAuthenticatingSystem(node);
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
