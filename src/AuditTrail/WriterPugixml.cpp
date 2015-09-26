#include "WriterPugixml.h"
#include "Log.h"

#include "pugixml.hpp"

using namespace pugi;

namespace IO
{

WriterPugiXml::WriterPugiXml(std::string filePath) : m_filePath(std::move(filePath)) {}

void iterateNodes(xml_node& xmlNode, const std::vector<Node>& nodes)
{
    for (const auto& node : nodes)
    {
        auto xmlChildNode = xmlNode.append_child(node.name().c_str());
        xmlNode.set_value(node.value().c_str());
        for (const auto& attribute : node.attributes())
        {
            auto xmlAttribute = xmlChildNode.append_attribute(attribute.name.c_str());
            xmlAttribute.set_value(attribute.value.c_str());
        }

        iterateNodes(xmlChildNode, node.nodes());
    }
}

void WriterPugiXml::write(const std::vector<Node>& nodes)
{
    if (nodes.empty())
        return;

    xml_document doc;
    auto rootNode = doc.append_child("AuditMessage");
    iterateNodes(rootNode, nodes);

    if (!doc.save_file(m_filePath.c_str()))
        Logger->error("Failed to save xml file '" + m_filePath + "'");
}

}