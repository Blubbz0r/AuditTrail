#include "EntityParticipantObject.h"

namespace AuditTrail
{

EntityParticipantObject::EntityParticipantObject(Type type, Role role, CodedValueType idTypeCode,
                                                 std::string objectId)
    : type(type),
      role(role),
      idTypeCode(idTypeCode),
      objectId(std::move(objectId))
{
}

IO::Node EntityParticipantObject::toNode() const
{
    IO::Node pariticpantObject("ParticipantObjectIdentification");
    pariticpantObject.addAttribute(IO::Attribute{"ParticipantObjectID", objectId});
    pariticpantObject.addAttribute(
        IO::Attribute{"ParticipantObjectTypeCode", std::to_string(static_cast<int>(type))});
    pariticpantObject.addAttribute(
        IO::Attribute{"ParticipantObjectTypeCodeRole", std::to_string(static_cast<int>(role))});

    pariticpantObject.addNode(idTypeCode.toNode());

    if (!objectNameOrQuery.empty())
        pariticpantObject.addNode(IO::Node{"ParticipantObjectName", objectNameOrQuery});

    if (!m_sopClasses.empty())
    {
        IO::Node objectDescription{"ParticipantObjectDescription"};

        for (const auto& sopClass : m_sopClasses)
        {
            IO::Node sopClassNode("SOPClass");
            sopClassNode.addAttribute(IO::Attribute{"UID", sopClass.uid});
            sopClassNode.addAttribute(
                IO::Attribute{"NumberOfInstances", std::to_string(sopClass.numberOfInstances)});
            objectDescription.addNode(sopClassNode);
        }

        pariticpantObject.addNode(objectDescription);
    }

    return pariticpantObject;
}

void EntityParticipantObject::addSOPClass(SOPClass sopClass)
{
    m_sopClasses.emplace_back(std::move(sopClass));
}

void EntityParticipantObject::setSOPClasses(std::vector<SOPClass> sopClasses)
{
    m_sopClasses = std::move(sopClasses);
}

}