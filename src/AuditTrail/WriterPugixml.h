#pragma once

#include "Writer.h"

namespace IO
{

/*!
    \brief  Writes Nodes to an xml file using pugixml.
*/
class WriterPugiXml : public Writer
{
public:
    WriterPugiXml(std::string filePath);

    void write(const std::vector<Node>& nodes) override;

private:
    std::string m_filePath;
};

}