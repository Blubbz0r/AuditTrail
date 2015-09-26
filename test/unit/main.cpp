#include "gmock/gmock.h"

#include "LogNoop.h"

Logging::Log* Logger = new Test::LogNoop;

int main(int argc, char** argv)
{
    testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
