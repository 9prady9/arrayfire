#include <af/array.h>
#include <Array.hpp>

namespace cuda
{
    template<typename T>
    Array<T> *shift(const Array<T> &in, const af::dim4 &sdims);
}
