#pragma once

#include "Writer.h"

#include <filesystem>

namespace IO
{

/*!
    \brief  Writes Nodes to an xml file using pugixml.
*/
class WriterPugiXml : public Writer
{
public:
    explicit WriterPugiXml(std::tr2::sys::path filePath);

    void write(const std::vector<Node>& nodes) override;

private:
    std::tr2::sys::path m_filePath;
};

}