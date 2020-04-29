/*******************************************************
 * Copyright (c) 2014, ArrayFire
 * All rights reserved.
 *
 * This file is distributed under 3-clause BSD license.
 * The complete license agreement can be obtained at:
 * http://arrayfire.com/licenses/BSD-3-Clause
 ********************************************************/

#include <copy.hpp>

#include <common/half.hpp>
#include <kernel/copy.hpp>
#include <platform.hpp>
#include <queue.hpp>

namespace cpu {
template<typename T>
void multiply_inplace(Array<T> &in, double val) {
    getQueue().enqueue(kernel::copyElemwise<T, T>, in, in, static_cast<T>(0),
                       val);
}

template<typename inType, typename outType>
Array<outType> resizeArray(const Array<inType> &in, const dim4 &outDims,
                           outType defaultValue, double scale) {
    Array<outType> out = createValueArray(outDims, defaultValue);
    getQueue().enqueue(kernel::copyElemwise<outType, inType>, out, in,
                       defaultValue, scale);
    return out;
}

#define INSTANTIATE(T) \
    template void multiply_inplace<T>(Array<T> & in, double norm);

INSTANTIATE(float)
INSTANTIATE(double)
INSTANTIATE(cfloat)
INSTANTIATE(cdouble)
INSTANTIATE(int)
INSTANTIATE(uint)
INSTANTIATE(uchar)
INSTANTIATE(char)
INSTANTIATE(intl)
INSTANTIATE(uintl)
INSTANTIATE(short)
INSTANTIATE(ushort)

#define INSTANTIATE_PAD_ARRAY(SRC_T)                                           \
    template Array<float> resizeArray<SRC_T, float>(                           \
        const Array<SRC_T> &, const dim4 &, float, double);                    \
    template Array<double> resizeArray<SRC_T, double>(                         \
        const Array<SRC_T> &, const dim4 &, double, double);                   \
    template Array<cfloat> resizeArray<SRC_T, cfloat>(                         \
        const Array<SRC_T> &, const dim4 &, cfloat, double);                   \
    template Array<cdouble> resizeArray<SRC_T, cdouble>(                       \
        const Array<SRC_T> &, const dim4 &, cdouble, double);                  \
    template Array<int> resizeArray<SRC_T, int>(const Array<SRC_T> &,          \
                                                const dim4 &, int, double);    \
    template Array<uint> resizeArray<SRC_T, uint>(const Array<SRC_T> &,        \
                                                  const dim4 &, uint, double); \
    template Array<intl> resizeArray<SRC_T, intl>(const Array<SRC_T> &,        \
                                                  const dim4 &, intl, double); \
    template Array<uintl> resizeArray<SRC_T, uintl>(                           \
        const Array<SRC_T> &, const dim4 &, uintl, double);                    \
    template Array<short> resizeArray<SRC_T, short>(                           \
        const Array<SRC_T> &, const dim4 &, short, double);                    \
    template Array<ushort> resizeArray<SRC_T, ushort>(                         \
        const Array<SRC_T> &, const dim4 &, ushort, double);                   \
    template Array<uchar> resizeArray<SRC_T, uchar>(                           \
        const Array<SRC_T> &, const dim4 &, uchar, double);                    \
    template Array<char> resizeArray<SRC_T, char>(const Array<SRC_T> &,        \
                                                  const dim4 &, char, double);

INSTANTIATE_PAD_ARRAY(float)
INSTANTIATE_PAD_ARRAY(double)
INSTANTIATE_PAD_ARRAY(int)
INSTANTIATE_PAD_ARRAY(uint)
INSTANTIATE_PAD_ARRAY(intl)
INSTANTIATE_PAD_ARRAY(uintl)
INSTANTIATE_PAD_ARRAY(uchar)
INSTANTIATE_PAD_ARRAY(char)
INSTANTIATE_PAD_ARRAY(ushort)
INSTANTIATE_PAD_ARRAY(short)
INSTANTIATE_PAD_ARRAY(common::half)

#define INSTANTIATE_PAD_ARRAY_COMPLEX(SRC_T)                 \
    template Array<cfloat> resizeArray<SRC_T, cfloat>(       \
        const Array<SRC_T> &, const dim4 &, cfloat, double); \
    template Array<cdouble> resizeArray<SRC_T, cdouble>(     \
        const Array<SRC_T> &, const dim4 &, cdouble, double);

INSTANTIATE_PAD_ARRAY_COMPLEX(cfloat)
INSTANTIATE_PAD_ARRAY_COMPLEX(cdouble)
}  // namespace cpu
