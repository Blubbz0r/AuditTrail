#include "STLUtils.h"

#include <sstream>

#include "DebugNew.h"

using namespace std;

namespace STLUtils
{

string join(const vector<std::string>& vec, const char* delimiter)
{
    std::string joined;
    auto it = vec.begin();
    while (it != vec.end())
    {
        // do not add delimiter before first string
        if (it != vec.begin())
            joined += delimiter;

        joined += *it;
        it++;
    }

    return joined;
}

}