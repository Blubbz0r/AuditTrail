#include "CodedValueTypeTests.h"

#include <gmock/gmock.h>

using namespace testing;

namespace Tests
{

void checkEventIdentification(const IO::Node& eventIdentification,
                              const std::string& eventActionCode,
                              AuditTrail::Outcome eventOutcomeIndicator)
{
    ASSERT_THAT(eventIdentification.attributes().size(), Eq(3));

    auto attribute = eventIdentification.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("EventActionCode"));
    EXPECT_THAT(attribute.value, Eq(eventActionCode));

    attribute = eventIdentification.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("EventDateTime"));
    // todo: how to check the date time?

    attribute = eventIdentification.attributes().at(2);
    ASSERT_THAT(attribute.name, Eq("EventOutcomeIndicator"));
    EXPECT_THAT(attribute.value, Eq(std::to_string(static_cast<int>(eventOutcomeIndicator))));
}

void checkCodedValueType(const IO::Node& eventId, const std::string& code,
                         const std::string& displayName)
{
    ASSERT_THAT(eventId.attributes().size(), Eq(3));

    auto attribute = eventId.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("code"));
    EXPECT_THAT(attribute.value, Eq(code));

    attribute = eventId.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("codeSystemName"));
    EXPECT_THAT(attribute.value, Eq("DCM"));

    attribute = eventId.attributes().at(2);
    ASSERT_THAT(attribute.name, Eq("displayName"));
    EXPECT_THAT(attribute.value, Eq(displayName));
}

void checkCodedValueType(const IO::Node& eventId, const std::string& code)
{
    ASSERT_THAT(eventId.attributes().size(), Eq(1));

    auto attribute = eventId.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("code"));
    EXPECT_THAT(attribute.value, Eq(code));
}

}
