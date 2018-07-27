/*******************************************************
 * Copyright (c) 2018, ArrayFire
 * All rights reserved.
 *
 * This file is distributed under 3-clause BSD license.
 * The complete license agreement can be obtained at:
 * http://arrayfire.com/licenses/BSD-3-Clause
 ********************************************************/

#include <chrono>
#include <string>
#include <sstream>

#define AF_BENCH_TIMER_START()  \
    auto start = std::chrono::high_resolution_clock::now();

#define AF_BENCH_TIMER_STOP()   \
    af::sync();                                                         \
    auto end = std::chrono::high_resolution_clock::now();               \
    auto elapsed_seconds =                                              \
            std::chrono::duration_cast<std::chrono::duration<double>>(  \
                    end - start);                                       \
    state.SetIterationTime(elapsed_seconds.count());

#define AF_GET_DEVICE_PROPS()  \
    char d_name[64];                                                    \
    char d_plat[64];                                                    \
    char d_tlkt[64];                                                    \
    char d_comp[64];                                                    \
    af::deviceprop(d_name, d_plat, d_tlkt, d_comp);                     \
    std::vector<std::string> info = { d_name, d_plat, d_tlkt, d_comp }; \

template <typename T>
std::string join(const T& v, const std::string& delim)
{
    std::ostringstream s;
    for (const auto& i : v) {
        if (&i != &v[0]) {
            s << delim;
        }
        s << i;
    }
    return s.str();
}
