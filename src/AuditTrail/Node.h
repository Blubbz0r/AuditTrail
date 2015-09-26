#pragma once

#include <string>
#include <vector>

namespace IO
{

struct Attribute
{
    std::string name;
    std::string value;
};

/*!
    \brief  Nodes together with Attributes represent our internal layout of Message data. It is a
            flexible concept that can easily be converted into the target format (e.g. XML).
*/
class Node
{
public:
    Node() = default;
    Node(std::string name) : m_name(std::move(name)) {}
    Node(std::string name, std::string value) : m_name(std::move(name)), m_value(std::move(value)) {}

    void setName(std::string name) { m_name = std::move(name); }
    std::string name() const { return m_name; }

    void setValue(std::string value) { m_value = std::move(value); }
    std::string value() const { return m_value; }

    void addAttribute(Attribute attribute) { m_attributes.emplace_back(std::move(attribute)); }
    const std::vector<Attribute>& attributes() const { return m_attributes; }

    void addNode(Node node) { m_nodes.emplace_back(std::move(node)); }
    const std::vector<Node>& nodes() const { return m_nodes; }

private:
    std::string m_name;
    std::string m_value;
    std::vector<Attribute> m_attributes;
    std::vector<Node> m_nodes;
};

}