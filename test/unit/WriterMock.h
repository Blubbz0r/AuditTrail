#include "gmock/gmock.h"

#include "Writer.h"

namespace Test
{

class WriterMock : public IO::Writer
{
public:
    MOCK_METHOD1(write, void(const std::vector<Node>&));
};

}