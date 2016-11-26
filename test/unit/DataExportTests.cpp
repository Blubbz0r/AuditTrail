#include "gmock/gmock.h"

#include "DataExport.h"
#include "CodedValueTypeTests.h"
#include "TestData.h"

using namespace AuditTrail;
using namespace IO;
using namespace Tests;
using namespace testing;

class DataExportTests : public Test
{
public:
    void checkRemoteUser(const Node& remoteUser);

    void checkExportingUser(const Node& exportingUser);

    void checkMedia(const Node& media);

    void checkStudy(const Node& study);
    void checkDescription(const Node& description);

    void checkPatient(const Node& patient);
};

TEST_F(DataExportTests, createNodes_WithAllAttributes_ReturnsCorrectNodes)
{
    DataExport dataExport(Outcome::Success, MediaType::DVD);

    dataExport.addRemoteUser(ActiveParticipant(User::ArbitraryUserID, false));
    dataExport.setExportingUser(ActiveParticipant(User::ArbitraryUserID, true));

    std::vector<SOPClass> sopClasses;
    sopClasses.emplace_back(SOPClass{ DICOM::ArbitrarySOPClassUID, DICOM::ArbitraryNumberOfInstances});
    dataExport.addStudy(DICOM::ArbitraryStudyInstanceUID, sopClasses);

    dataExport.addPatient(DICOM::ArbitraryPatientID, DICOM::ArbitraryPatientName);

    auto nodes = dataExport.createNodes();

    ASSERT_THAT(nodes.size(), Eq(6));

    auto eventIdentification = nodes[0];
    ASSERT_THAT(eventIdentification.name(), Eq("EventIdentification"));
    checkEventIdentification(eventIdentification, "R", Outcome::Success);

    ASSERT_THAT(eventIdentification.nodes().size(), Eq(1));

    auto eventId = eventIdentification.nodes().at(0);
    ASSERT_THAT(eventId.name(), Eq("EventID"));
    checkCodedValueType(eventId, "110106", "Export");

    auto node = nodes[1];
    ASSERT_THAT(node.name(), Eq("ActiveParticipant"));
    checkRemoteUser(node);

    node = nodes[2];
    ASSERT_THAT(node.name(), Eq("ActiveParticipant"));
    checkExportingUser(node);

    node = nodes[3];
    ASSERT_THAT(node.name(), Eq("ActiveParticipant"));
    checkMedia(node);

    node = nodes[4];
    ASSERT_THAT(node.name(), Eq("ParticipantObjectIdentification"));
    checkStudy(node);

    node = nodes[5];
    ASSERT_THAT(node.name(), Eq("ParticipantObjectIdentification"));
    checkPatient(node);
}

void DataExportTests::checkRemoteUser(const Node& remoteUser)
{
    ASSERT_THAT(remoteUser.attributes().size(), Eq(2));

    auto attribute = remoteUser.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("UserID"));
    EXPECT_THAT(attribute.value, Eq("john.doe@gmail.com"));

    attribute = remoteUser.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("UserIsRequestor"));
    EXPECT_THAT(attribute.value, Eq("false"));

    ASSERT_THAT(remoteUser.nodes().size(), Eq(1));

    auto node = remoteUser.nodes().at(0);
    ASSERT_THAT(node.name(), Eq("RoleIDCode"));
    checkCodedValueType(node, "110152", "Destination Role ID");
}

void DataExportTests::checkExportingUser(const Node& exportingUser)
{
    ASSERT_THAT(exportingUser.attributes().size(), Eq(2));

    auto attribute = exportingUser.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("UserID"));
    EXPECT_THAT(attribute.value, Eq(User::ArbitraryUserID));

    attribute = exportingUser.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("UserIsRequestor"));
    EXPECT_THAT(attribute.value, Eq("true"));

    ASSERT_THAT(exportingUser.nodes().size(), Eq(1));

    auto node = exportingUser.nodes().at(0);
    ASSERT_THAT(node.name(), Eq("RoleIDCode"));
    checkCodedValueType(node, "110153", "Source Role ID");
}

void DataExportTests::checkMedia(const Node& media)
{
    ASSERT_THAT(media.attributes().size(), Eq(2));

    auto attribute = media.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("UserID"));
    EXPECT_THAT(attribute.value, Eq("DVD"));

    attribute = media.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("UserIsRequestor"));
    EXPECT_THAT(attribute.value, Eq("false"));

    ASSERT_THAT(media.nodes().size(), Eq(1));

    auto node = media.nodes().at(0);
    ASSERT_THAT(node.name(), Eq("RoleIDCode"));
    checkCodedValueType(node, "110154", "Destination Media");
}

void DataExportTests::checkStudy(const Node& study)
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

void DataExportTests::checkDescription(const Node& description)
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

void DataExportTests::checkPatient(const Node& patient)
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
