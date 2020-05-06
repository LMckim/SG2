#ifndef SG_TOOL_DEBUG
#define SG_TOOL_DEBUG

#include <string>
#include <iostream>

namespace SG::Tool
{
    using std::string;
    void logOut(string log)
    {
        std::cout << log << std::endl;
    }
}


#endif