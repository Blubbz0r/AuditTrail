#include "gmock/gmock.h"

#include "DataExport.h"

using namespace AuditTrail;
using namespace IO;
using namespace testing;

class DataExportTests : public Test
{
public:
    void checkEventIdentification(const Node& eventIdentification);
    void checkEventId(const Node& eventId);

    void checkRemoteUser(const Node& remoteUser);
    void checkRoleIDCodeRemoteUser(const Node& roleIDCode);

    void checkExportingUser(const Node& exportingUser);
    void checkRoleIDCodeExportingUser(const Node& roleIDCode);

    void checkMedia(const Node& media);
    void checkRoleIDCodeMedia(const Node& roleIDCode);

    void checkStudy(const Node& study);
    void checkStudyIdTypeCode(const Node& idTypeCode);
    void checkDescription(const Node& description);

    void checkPatient(const Node& patient);
    void checkPatientIDTypeCode(const Node& idTypeCode);
};

TEST_F(DataExportTests, createNodes_WithAllAttributes_ReturnsCorrectNodes)
{
    DataExport dataExport(Outcome::Success, MediaType::DVD);

    dataExport.addRemoteUser(ActiveParticipant("habrich@image-systems.local", false));
    dataExport.setExportingUser(ActiveParticipant("habrich@image-systems.local", true));

    std::vector<SOPClass> sopClasses;
    sopClasses.emplace_back(SOPClass{ "1.2.840.10008.5.1.4.1.1.2", 1000 });
    dataExport.addStudy("1.2.2.55.555.84984456465", sopClasses);

    dataExport.addPatient("ID123", "Sandra Newman");

    auto nodes = dataExport.createNodes();

    ASSERT_THAT(nodes.size(), Eq(6));

    auto node = nodes[0];
    ASSERT_THAT(node.name(), Eq("EventIdentification"));
    checkEventIdentification(node);

    node = nodes[1];
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

void DataExportTests::checkEventIdentification(const Node& eventIdentification)
{
    ASSERT_THAT(eventIdentification.attributes().size(), Eq(3));

    auto attribute = eventIdentification.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("EventActionCode"));
    EXPECT_THAT(attribute.value, Eq("R"));

    attribute = eventIdentification.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("EventDateTime"));
    // todo: how to check the date time?

    attribute = eventIdentification.attributes().at(2);
    ASSERT_THAT(attribute.name, Eq("EventOutcomeIndicator"));
    EXPECT_THAT(attribute.value, Eq(std::to_string(static_cast<int>(Outcome::Success))));

    ASSERT_THAT(eventIdentification.nodes().size(), Eq(1));

    auto node = eventIdentification.nodes().at(0);
    ASSERT_THAT(node.name(), Eq("EventID"));
    checkEventId(node);
}

void DataExportTests::checkEventId(const Node& eventId)
{
    ASSERT_THAT(eventId.attributes().size(), Eq(3));

    auto attribute = eventId.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("code"));
    EXPECT_THAT(attribute.value, Eq("110106"));

    attribute = eventId.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("codeSystemName"));
    EXPECT_THAT(attribute.value, Eq("DCM"));

    attribute = eventId.attributes().at(2);
    ASSERT_THAT(attribute.name, Eq("displayName"));
    EXPECT_THAT(attribute.value, Eq("Export"));
}

void DataExportTests::checkRemoteUser(const Node& remoteUser)
{
    ASSERT_THAT(remoteUser.attributes().size(), Eq(2));

    auto attribute = remoteUser.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("UserID"));
    EXPECT_THAT(attribute.value, Eq("habrich@image-systems.local"));

    attribute = remoteUser.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("UserIsRequestor"));
    EXPECT_THAT(attribute.value, Eq("false"));

    ASSERT_THAT(remoteUser.nodes().size(), Eq(1));

    auto node = remoteUser.nodes().at(0);
    ASSERT_THAT(node.name(), Eq("RoleIDCode"));
    checkRoleIDCodeRemoteUser(node);
}

void DataExportTests::checkRoleIDCodeRemoteUser(const Node& roleIDCode)
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

void DataExportTests::checkExportingUser(const Node& exportingUser)
{
    ASSERT_THAT(exportingUser.attributes().size(), Eq(2));

    auto attribute = exportingUser.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("UserID"));
    EXPECT_THAT(attribute.value, Eq("habrich@image-systems.local"));

    attribute = exportingUser.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("UserIsRequestor"));
    EXPECT_THAT(attribute.value, Eq("true"));

    ASSERT_THAT(exportingUser.nodes().size(), Eq(1));

    auto node = exportingUser.nodes().at(0);
    ASSERT_THAT(node.name(), Eq("RoleIDCode"));
    checkRoleIDCodeExportingUser(node);
}

void DataExportTests::checkRoleIDCodeExportingUser(const Node& roleIDCode)
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
    checkRoleIDCodeMedia(node);
}

void DataExportTests::checkRoleIDCodeMedia(const Node& roleIDCode)
{
    ASSERT_THAT(roleIDCode.attributes().size(), Eq(3));

    auto attribute = roleIDCode.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("code"));
    EXPECT_THAT(attribute.value, Eq("110154"));

    attribute = roleIDCode.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("codeSystemName"));
    EXPECT_THAT(attribute.value, Eq("DCM"));

    attribute = roleIDCode.attributes().at(2);
    ASSERT_THAT(attribute.name, Eq("displayName"));
    EXPECT_THAT(attribute.value, Eq("Destination Media"));
}

void DataExportTests::checkStudy(const Node& study)
{
    ASSERT_THAT(study.attributes().size(), Eq(3));

    auto attribute = study.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("ParticipantObjectID"));
    EXPECT_THAT(attribute.value, Eq("1.2.2.55.555.84984456465"));

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

void DataExportTests::checkStudyIdTypeCode(const Node& idTypeCode)
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

void DataExportTests::checkDescription(const Node& description)
{
    ASSERT_THAT(description.attributes().size(), Eq(0));

    ASSERT_THAT(description.nodes().size(), Eq(1));

    auto node = description.nodes().at(0);
    ASSERT_THAT(node.name(), Eq("SOPClass"));

    ASSERT_THAT(node.attributes().size(), Eq(2));
    auto attribute = node.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("UID"));
    EXPECT_THAT(attribute.value, Eq("1.2.840.10008.5.1.4.1.1.2"));

    attribute = node.attributes().at(1);
    ASSERT_THAT(attribute.name, Eq("NumberOfInstances"));
    EXPECT_THAT(attribute.value, Eq("1000"));
}

void DataExportTests::checkPatient(const Node& patient)
{
    ASSERT_THAT(patient.attributes().size(), Eq(3));

    auto attribute = patient.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("ParticipantObjectID"));
    EXPECT_THAT(attribute.value, Eq("ID123"));

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
    ASSERT_THAT(node.value(), Eq("Sandra Newman"));
}

void DataExportTests::checkPatientIDTypeCode(const Node& idTypeCode)
{
    ASSERT_THAT(idTypeCode.attributes().size(), Eq(1));

    auto attribute = idTypeCode.attributes().at(0);
    ASSERT_THAT(attribute.name, Eq("code"));
    EXPECT_THAT(attribute.value, Eq("2"));
}
