#include "gmock/gmock.h"

#include "DataImport.h"

using namespace AuditTrail;
using namespace IO;
using namespace testing;

class DataImportTests : public Test
{
public:
    void checkEventIdentification(const Node& eventIdentification);
    void checkEventId(const Node& eventId);

    void checkImportingUser(const Node& importingUser);
    void checkRoleIDCodeImportingUser(const Node& roleIDCode);

    void checkSourceMedia(const Node& sourceMedia);
    void checkRoleIDCodeSourceMedia(const Node& roleIDCode);

    void checkSource(const Node& source);
    void checkRoleIDCodeSource(const Node& roleIDCode);
};

TEST_F(DataImportTests, createNodes_WithAllAttributes_ReturnsCorrectNodes)
{
    DataImport dataImport(Outcome::MinorFailure, MediaType::CD);
    dataImport.addImportingUser(ActiveParticipant("john.doe@gmail.com", true));
    dataImport.addSource(ActiveParticipant("john.doe@gmail.com", false));

    auto nodes = dataImport.createNodes();

    ASSERT_THAT(nodes.size(), Eq(4));

    auto node = nodes[0];
    ASSERT_THAT(node.name(), Eq("EventIdentification"));
    checkEventIdentification(node);

    node = nodes[1];
    ASSERT_THAT(node.name(), Eq("ActiveParticipant"));
    checkImportingUser(node);

    node = nodes[2];
    ASSERT_THAT(node.name(), Eq("ActiveParticipant"));
    checkSourceMedia(node);

    node = nodes[3];
    ASSERT_THAT(node.name(), Eq("ActiveParticipant"));
    checkSource(node);
}

void DataImportTests::checkEventIdentification(const Node& eventIdentification)
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
    EXPECT_THAT(attribute.value, Eq(std::to_string(static_cast<int>(Outcome::MinorFailure))));

    ASSERT_THAT(eventIdentification.nodes().size(), Eq(1));

    auto node = eventIdentification.nodes().at(0);
    ASSERT_THAT(node.name(), Eq("EventID"));
    checkEventId(node);
}

void DataImportTests::checkEventId(const Node& eventId)
{
    ASSERT_THAT(eventId.attributes().size(), Eq(3));

    auto attribute = eventId.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("code"));
    EXPECT_THAT(attribute.value, Eq("110107"));

    attribute = eventId.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("codeSystemName"));
    EXPECT_THAT(attribute.value, Eq("DCM"));

    attribute = eventId.attributes().at(2);
    ASSERT_THAT(attribute.name, Eq("displayName"));
    EXPECT_THAT(attribute.value, Eq("Import"));
}

void DataImportTests::checkImportingUser(const Node& importingUser)
{
    ASSERT_THAT(importingUser.attributes().size(), Eq(2));

    auto attribute = importingUser.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("UserID"));
    EXPECT_THAT(attribute.value, Eq("john.doe@gmail.com"));

    attribute = importingUser.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("UserIsRequestor"));
    EXPECT_THAT(attribute.value, Eq("true"));

    ASSERT_THAT(importingUser.nodes().size(), Eq(1));

    auto node = importingUser.nodes().at(0);
    ASSERT_THAT(node.name(), Eq("RoleIDCode"));
    checkRoleIDCodeImportingUser(node);
}

void DataImportTests::checkRoleIDCodeImportingUser(const Node& roleIDCode)
{
    ASSERT_THAT(roleIDCode.attributes().size(), Eq(3));

    auto attribute = roleIDCode.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("code"));
    EXPECT_THAT(attribute.value, Eq("110152"));

    attribute = roleIDCode.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("codeSystemName"));
    EXPECT_THAT(attribute.value, Eq("DCM"));

    attribute = roleIDCode.attributes().at(2);
    ASSERT_THAT(attribute.name, Eq("displayName"));
    EXPECT_THAT(attribute.value, Eq("Destination Role ID"));
}

void DataImportTests::checkSourceMedia(const Node& sourceMedia)
{
    ASSERT_THAT(sourceMedia.attributes().size(), Eq(2));

    auto attribute = sourceMedia.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("UserID"));
    EXPECT_THAT(attribute.value, Eq("CD"));

    attribute = sourceMedia.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("UserIsRequestor"));
    EXPECT_THAT(attribute.value, Eq("false"));

    ASSERT_THAT(sourceMedia.nodes().size(), Eq(1));

    auto node = sourceMedia.nodes().at(0);
    ASSERT_THAT(node.name(), Eq("RoleIDCode"));
    checkRoleIDCodeSourceMedia(node);
}

void DataImportTests::checkRoleIDCodeSourceMedia(const Node& roleIDCode)
{
    ASSERT_THAT(roleIDCode.attributes().size(), Eq(3));

    auto attribute = roleIDCode.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("code"));
    EXPECT_THAT(attribute.value, Eq("110155"));

    attribute = roleIDCode.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("codeSystemName"));
    EXPECT_THAT(attribute.value, Eq("DCM"));

    attribute = roleIDCode.attributes().at(2);
    ASSERT_THAT(attribute.name, Eq("displayName"));
    EXPECT_THAT(attribute.value, Eq("Source Media"));
}

void DataImportTests::checkSource(const Node& source)
{
    ASSERT_THAT(source.attributes().size(), Eq(2));

    auto attribute = source.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("UserID"));
    EXPECT_THAT(attribute.value, Eq("john.doe@gmail.com"));

    attribute = source.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("UserIsRequestor"));
    EXPECT_THAT(attribute.value, Eq("false"));

    ASSERT_THAT(source.nodes().size(), Eq(1));

    auto node = source.nodes().at(0);
    ASSERT_THAT(node.name(), Eq("RoleIDCode"));
    checkRoleIDCodeSource(node);
}

void DataImportTests::checkRoleIDCodeSource(const Node& roleIDCode)
{
    ASSERT_THAT(roleIDCode.attributes().size(), Eq(3));

    auto attribute = roleIDCode.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("code"));
    EXPECT_THAT(attribute.value, Eq("110153"));

    attribute = roleIDCode.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("codeSystemName"));
    EXPECT_THAT(attribute.value, Eq("DCM"));

    attribute = roleIDCode.attributes().at(2);
    ASSERT_THAT(attribute.name, Eq("displayName"));
    EXPECT_THAT(attribute.value, Eq("Source Role ID"));
}
