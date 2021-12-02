#pragma once

#include <map>
#include <chrono>
#include <cmath>
#include <string>


class ArgInfo
{
    using milli = std::chrono::milliseconds;

  public:
    milli frame_limit = milli(0); // minimum number of ms between frames
};


// not ideal; skips non-dash-prepended options and dash-prepended args. good enough.
ArgInfo parse(int argc, const char** argv)
{
    ArgInfo info { };
    std::string curr;
    std::string arg;
    for (int i = 0; i < argc; i++)
    {
        curr = argv[i];

        if (curr[0] != '-')
            continue;

        if (curr == "-f" || curr == "--fps" || curr == "--frame-limit")
        {
            // overrides frame_limit only if it is currently 0
            // and there is a next argument which is not an option
            if (info.frame_limit.count() == 0 && i != argc - 1 && argv[i + 1][0] != '-')
            {
                using ms = std::chrono::milliseconds;
                using rep = typename ms::rep;
                arg = argv[i + 1];
                rep value = 0;
                try
                {
                    value = std::stoi(arg);
                    // convert from frames per second to milliseconds per frame
                    info.frame_limit = ms(rep(std::ceil(1000 / value)));
                }
                catch (const std::invalid_argument&) { /* do nothing */ }
            }
        }
    }

    return info;
}
