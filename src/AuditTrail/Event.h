#pragma once

namespace AuditTrail
{

enum class Outcome
{
    Success = 0,
    MinorFailure = 4,
    SeriousFailure = 8,
    MajorFailure = 12
};

}