#pragma once
#include <string>
#include <iostream>

extern "C"
{
#include "libavutil/error.h"
#include "libavutil/rational.h"
}

static std::string errstr(int err)
{
    char buf[1024] = { 0 };
    if (av_strerror(err, buf, sizeof(buf) - 1) < 0)
    {
        std::cerr << "description for errnum cannot be found!" << std::endl;
    }
    return buf;
}

static double r2d(AVRational r)
{
    return r.den == 0 ? 0 : (double)r.num / (double)r.den;
}