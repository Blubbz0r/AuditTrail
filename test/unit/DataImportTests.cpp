#include "gmock/gmock.h"

#include "DataImport.h"
#include "CodedValueTypeTests.h"
#include "TestData.h"

using namespace AuditTrail;
using namespace IO;
using namespace Tests;
using namespace testing;

class DataImportTests : public Test
{
public:
    void checkImportingUser(const Node& importingUser);

    void checkSourceMedia(const Node& sourceMedia);

    void checkSource(const Node& source);

    void checkStudy(const Node& study);
    void checkDescription(const Node& description);

    void checkPatient(const Node& patient);
};

TEST_F(DataImportTests, createNodes_WithAllAttributes_ReturnsCorrectNodes)
{
    DataImport dataImport(Outcome::MinorFailure, MediaType::CD);
    dataImport.addImportingUser(ActiveParticipant(User::ArbitraryUserID, true));
    dataImport.addSource(ActiveParticipant(User::ArbitraryUserID, false));

    std::vector<SOPClass> sopClasses;
    sopClasses.emplace_back(SOPClass{DICOM::ArbitrarySOPClassUID, DICOM::ArbitraryNumberOfInstances});
    dataImport.addStudy(DICOM::ArbitraryStudyInstanceUID, sopClasses);

    dataImport.addPatient(DICOM::ArbitraryPatientID, DICOM::ArbitraryPatientName);

    auto nodes = dataImport.createNodes();

    ASSERT_THAT(nodes.size(), Eq(6));

    auto eventIdentification = nodes[0];
    ASSERT_THAT(eventIdentification.name(), Eq("EventIdentification"));
    checkEventIdentification(eventIdentification, "C", Outcome::MinorFailure);

    ASSERT_THAT(eventIdentification.nodes().size(), Eq(1));

    auto eventId = eventIdentification.nodes().at(0);
    ASSERT_THAT(eventId.name(), Eq("EventID"));
    checkCodedValueType(eventId, "110107", "Import");

    auto node = nodes[1];
    ASSERT_THAT(node.name(), Eq("ActiveParticipant"));
    checkImportingUser(node);

    node = nodes[2];
    ASSERT_THAT(node.name(), Eq("ActiveParticipant"));
    checkSourceMedia(node);

    node = nodes[3];
    ASSERT_THAT(node.name(), Eq("ActiveParticipant"));
    checkSource(node);

    node = nodes[4];
    ASSERT_THAT(node.name(), Eq("ParticipantObjectIdentification"));
    checkStudy(node);

    node = nodes[5];
    ASSERT_THAT(node.name(), Eq("ParticipantObjectIdentification"));
    checkPatient(node);
}

void DataImportTests::checkImportingUser(const Node& importingUser)
{
    ASSERT_THAT(importingUser.attributes().size(), Eq(2));

    auto attribute = importingUser.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("UserID"));
    EXPECT_THAT(attribute.value, Eq(User::ArbitraryUserID));

    attribute = importingUser.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("UserIsRequestor"));
    EXPECT_THAT(attribute.value, Eq("true"));

    ASSERT_THAT(importingUser.nodes().size(), Eq(1));

    auto node = importingUser.nodes().at(0);
    ASSERT_THAT(node.name(), Eq("RoleIDCode"));
    checkCodedValueType(node, "110152", "Destination Role ID");
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
    checkCodedValueType(node, "110155", "Source Media");
}

void DataImportTests::checkSource(const Node& source)
{
    ASSERT_THAT(source.attributes().size(), Eq(2));

    auto attribute = source.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("UserID"));
    EXPECT_THAT(attribute.value, Eq(User::ArbitraryUserID));

    attribute = source.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("UserIsRequestor"));
    EXPECT_THAT(attribute.value, Eq("false"));

    ASSERT_THAT(source.nodes().size(), Eq(1));

    auto node = source.nodes().at(0);
    ASSERT_THAT(node.name(), Eq("RoleIDCode"));
    checkCodedValueType(node, "110153", "Source Role ID");
}

void DataImportTests::checkStudy(const Node& study)
{
    ASSERT_THAT(study.attributes().size(), Eq(3));

    auto attribute = study.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("ParticipantObjectID"));
    EXPECT_THAT(attribute.value, Eq(DICOM::ArbitraryStudyInstanceUID));

    attribute = study.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("ParticipantObjectTypeCode"));
    EXPECT_THAT(attribute.value, Eq("2"));

    attribute = study.attributes().at(2);
    ASSERT_THAT(attribute.name, Eq("ParticipantObjectTypeCodeRole"));
    EXPECT_THAT(attribute.value, Eq("3"));

    ASSERT_THAT(study.nodes().size(), Eq(2));
    auto node = study.nodes().at(0);
    ASSERT_THAT(node.name(), Eq("ParticipantObjectIDTypeCode"));
    checkCodedValueType(node, "110180", "Study Instance UID");

    node = study.nodes().at(1);
    ASSERT_THAT(node.name(), Eq("ParticipantObjectDescription"));
    checkDescription(node);
}

void DataImportTests::checkDescription(const Node& description)
{
    ASSERT_THAT(description.attributes().size(), Eq(0));

    ASSERT_THAT(description.nodes().size(), Eq(1));

    auto node = description.nodes().at(0);
    ASSERT_THAT(node.name(), Eq("SOPClass"));

    ASSERT_THAT(node.attributes().size(), Eq(2));
    auto attribute = node.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("UID"));
    EXPECT_THAT(attribute.value, Eq(DICOM::ArbitrarySOPClassUID));

    attribute = node.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("NumberOfInstances"));
    EXPECT_THAT(attribute.value, Eq(std::to_string(DICOM::ArbitraryNumberOfInstances)));
}

void DataImportTests::checkPatient(const Node& patient)
{
    ASSERT_THAT(patient.attributes().size(), Eq(3));

    auto attribute = patient.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("ParticipantObjectID"));
    EXPECT_THAT(attribute.value, Eq(DICOM::ArbitraryPatientID));

    attribute = patient.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("ParticipantObjectTypeCode"));
    EXPECT_THAT(attribute.value, Eq("1"));

    attribute = patient.attributes().at(2);
    ASSERT_THAT(attribute.name, Eq("ParticipantObjectTypeCodeRole"));
    EXPECT_THAT(attribute.value, Eq("1"));

    ASSERT_THAT(patient.nodes().size(), Eq(2));
    auto node = patient.nodes().at(0);
    ASSERT_THAT(node.name(), Eq("ParticipantObjectIDTypeCode"));
    checkCodedValueType(node, "2");

    node = patient.nodes().at(1);
    ASSERT_THAT(node.name(), Eq("ParticipantObjectName"));
    ASSERT_THAT(node.value(), Eq(DICOM::ArbitraryPatientName));
}
