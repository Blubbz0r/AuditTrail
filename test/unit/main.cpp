#include "gmock/gmock.h"

#include "LogNoop.h"

std::unique_ptr<Logging::Log> Logger = std::make_unique<Test::LogNoop>();

int main(int argc, char** argv)
{
    testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
