/*******************************************************
 * Copyright (c) 2017, ArrayFire
 * All rights reserved.
 *
 * This file is distributed under 3-clause BSD license.
 * The complete license agreement can be obtained at:
 * http://arrayfire.com/licenses/BSD-3-Clause
 ********************************************************/

#pragma once
#include <af/defines.h>

#ifdef __cplusplus
namespace af
{
class array;
// Let's write C++ API once C-API is ironed out
}
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if AF_API_VERSION >= 36
/**
 Simple mean squares metric parameters

 \ingroup metrics_img
 */
typedef struct {
    //TODO: Verify once more; Don't think there are any special attributes
    bool        use_fixed_image_gradient_filter;
    bool        use_moving_image_gradient_filter;
    bool        use_float_correction;
} af_means_square_metric;

/**
 Mattes Mutual Information parameters

 \ingroup metrics_img
 */
typedef struct {
    //TODO: Verify once more; Don't think there are any special attributes
    unsigned    number_of_bins;
    bool        use_fixed_image_gradient_filter;
    bool        use_moving_image_gradient_filter;
    bool        use_float_correction;
} af_mattes_mutual_information_metric;

/**
 Generic metric structure

 This objects wraps a specific error metric details

 \ingroup metrics_img
 */
typedef struct {
    af_metric_type type;

    /// metric should point to any one of the above
    /// af_*_metric structures
    void* metric;
} af_metric;
#endif

#ifdef __cplusplus
}
#endif
