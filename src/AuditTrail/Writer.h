#pragma once

#include "Node.h"

namespace IO
{

/*!
    \brief  Abstract interface for writing Nodes to an output device.
*/
class Writer
{
public:
    virtual ~Writer() {}

    virtual void write(const std::vector<Node>& nodes) = 0;
};

}