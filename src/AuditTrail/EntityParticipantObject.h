#pragma once

#include "Entity.h"
#include "CodedValueType.h"

namespace AuditTrail
{

struct SOPClass
{
    std::string uid;
    int numberOfInstances;
};

struct EntityParticipantObject : public Entity
{
    enum class Type
    {
        Person = 1,
        SystemObject,
        Organization,
        Other
    };

    enum class Role
    {
        Patient = 1,
        Location,
        Report,
        Resource,
        MasterFile,
        User,
        List,
        Doctor,
        Subscriber,
        Guarantor,
        SecurityUserEntity,
        SecurityUserGroup,
        SecurityResource,
        SecurityGranulatiryDefinition,
        Provider,
        ReportDestination,
        ReportLibrary,
        Schedule,
        Customer,
        Job,
        JobStream,
        Table,
        RoutingCriteria,
        Query
    };

    EntityParticipantObject(Type type, Role role, CodedValueType idTypeCode, std::string objectId);

    IO::Node toNode() const override;

    Type type;
    Role role;
    CodedValueType idTypeCode;
    std::string objectId;
    std::string objectNameOrQuery;

    enum class DataLifeCycle
    {
        Origination = 1,
        Creation = 1,
        Import = 2,
        Copy = 2,
        Amendment,
        Verification,
        Translation,
        Access = 6,
        Use = 6,
        Deidentification,
        Aggregation = 8,
        Summarization = 8,
        Derivation = 8,
        Report,
        Export,
        Disclosure,
        ReceiptOfDisclosure,
        Archiving,
        LogicalDeletion,
        PermanentErasure = 15,
        PhysicalDestruction = 15
    };

    DataLifeCycle dataLifeCycle;

    void addSOPClass(SOPClass sopClass);
    void setSOPClasses(std::vector<SOPClass> sopClasses);

private:
    std::vector<SOPClass> m_sopClasses;
};

}