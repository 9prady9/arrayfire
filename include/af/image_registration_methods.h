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
 Greedy SyN Image Registration Method

 \ingroup registration_methods
 */
typedef struct {
    double      learning_rate;
    unsigned    iterations_per_level;
    double      convergence_threshold;
    size_t      convergence_window_size;
    bool        downsample_images_for_metric_derivatives;
    bool        average_mid_point_gradients;
    double      smoothing_variance_for_update_field;
    double      smoothing_variance_for_total_field;
    //FIXME double check for missing attributes
} af_syn_image_registration;

/**
 Greedy B-Spline SyN Image Registration Method

 \ingroup registration_methods
 */
typedef af_syn_image_registration af_bspline_syn_image_registration;

//Uncomment when enabled
//typedef struct {
//    double      learning_rate;
//    unsigned    iterations_per_level;
//    double      convergence_threshold;
//    size_t      convergence_window_size;
//    size_t      number_of_timepoint_samples;
//} af_varying_bspline_vfield_registration;
//
//typedef af_varying_bspline_vfield_registration af_varying_vfield_registration;

/**
 Generic registration structure

 This objects wraps a specific registration method

 \ingroup registration_methods
 */
typedef struct {
    af_registration_type type;

    /// registration should point to any one of the above
    /// af_*_registration structures
    void* registration;
} af_registration_method;
#endif

#ifdef __cplusplus
}
#endif
