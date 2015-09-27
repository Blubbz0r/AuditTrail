#include "gmock/gmock.h"

#include "StudyDeleted.h"
#include "TestData.h"

using namespace AuditTrail;
using namespace IO;
using namespace Tests;
using namespace testing;

class StudyDeletedTests : public Test
{
public:
    void checkEventIdentification(const Node& eventIdentification);
    void checkEventId(const Node& eventId);

    void checkDeletingPerson(const Node& deletingPerson);

    void checkStudy(const Node& study);
    void checkStudyIdTypeCode(const Node& idTypeCode);
    void checkDescription(const Node& description);

    void checkPatient(const Node& patient);
    void checkPatientIDTypeCode(const Node& idTypeCode);
};

TEST_F(StudyDeletedTests, createNodes_WithAllAttributes_ReturnsCorrectNodes)
{
    StudyDeleted studyDeleted(Outcome::MinorFailure, DICOM::ArbitraryPatientID);
    studyDeleted.setDeletingPerson(ActiveParticipant(User::ArbitraryUserID, true));

    std::vector<SOPClass> sopClasses;
    sopClasses.emplace_back(SOPClass{DICOM::ArbitrarySOPClassUID, DICOM::ArbitraryNumberOfInstances});
    studyDeleted.addStudy(DICOM::ArbitraryStudyInstanceUID, sopClasses);

    studyDeleted.setPatientName(DICOM::ArbitraryPatientName);

    auto nodes = studyDeleted.createNodes();

    ASSERT_THAT(nodes.size(), Eq(4));

    auto node = nodes[0];
    ASSERT_THAT(node.name(), Eq("EventIdentification"));
    checkEventIdentification(node);

    node = nodes[1];
    ASSERT_THAT(node.name(), Eq("ActiveParticipant"));
    checkDeletingPerson(node);

    node = nodes[2];
    ASSERT_THAT(node.name(), Eq("ParticipantObjectIdentification"));
    checkStudy(node);

    node = nodes[3];
    ASSERT_THAT(node.name(), Eq("ParticipantObjectIdentification"));
    checkPatient(node);
}

void StudyDeletedTests::checkEventIdentification(const Node& eventIdentification)
{
    ASSERT_THAT(eventIdentification.attributes().size(), Eq(3));

    auto attribute = eventIdentification.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("EventActionCode"));
    EXPECT_THAT(attribute.value, Eq("D"));

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

void StudyDeletedTests::checkEventId(const Node& eventId)
{
    ASSERT_THAT(eventId.attributes().size(), Eq(3));

    auto attribute = eventId.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("code"));
    EXPECT_THAT(attribute.value, Eq("110105"));

    attribute = eventId.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("codeSystemName"));
    EXPECT_THAT(attribute.value, Eq("DCM"));

    attribute = eventId.attributes().at(2);
    ASSERT_THAT(attribute.name, Eq("displayName"));
    EXPECT_THAT(attribute.value, Eq("DICOM Study Deleted"));
}

void StudyDeletedTests::checkDeletingPerson(const Node& deletingPerson)
{
    ASSERT_THAT(deletingPerson.attributes().size(), Eq(2));

    auto attribute = deletingPerson.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("UserID"));
    EXPECT_THAT(attribute.value, Eq(User::ArbitraryUserID));

    attribute = deletingPerson.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("UserIsRequestor"));
    EXPECT_THAT(attribute.value, Eq("true"));

    ASSERT_THAT(deletingPerson.nodes().size(), Eq(0));
}

void StudyDeletedTests::checkStudy(const Node& study)
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
    checkStudyIdTypeCode(node);

    node = study.nodes().at(1);
    ASSERT_THAT(node.name(), Eq("ParticipantObjectDescription"));
    checkDescription(node);
}

void StudyDeletedTests::checkStudyIdTypeCode(const Node& idTypeCode)
{
    ASSERT_THAT(idTypeCode.attributes().size(), Eq(3));

    auto attribute = idTypeCode.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("code"));
    EXPECT_THAT(attribute.value, Eq("110180"));

    attribute = idTypeCode.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("codeSystemName"));
    EXPECT_THAT(attribute.value, Eq("DCM"));

    attribute = idTypeCode.attributes().at(2);
    ASSERT_THAT(attribute.name, Eq("displayName"));
    EXPECT_THAT(attribute.value, Eq("Study Instance UID"));
}

void StudyDeletedTests::checkDescription(const Node& description)
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

void StudyDeletedTests::checkPatient(const Node& patient)
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
    checkPatientIDTypeCode(node);

    node = patient.nodes().at(1);
    ASSERT_THAT(node.name(), Eq("ParticipantObjectName"));
    ASSERT_THAT(node.value(), Eq(DICOM::ArbitraryPatientName));
}

void StudyDeletedTests::checkPatientIDTypeCode(const Node& idTypeCode)
{
    ASSERT_THAT(idTypeCode.attributes().size(), Eq(1));

    auto attribute = idTypeCode.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("code"));
    EXPECT_THAT(attribute.value, Eq("2"));
}
