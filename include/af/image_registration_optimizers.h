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
 Regular Step Gradient Descent Parameters

 \ingroup optimizers_img
 */
typedef struct {
    //TODO: Verify once more; Don't think there are any special attributes
    double      minimum_step_length;
    double      relaxation_factor;
    double      gradient_magintude_tolerance;
    size_t      window_convergence_size;
    double      maximum_step_size_physical_units;
    double      learning_rate;
    bool        estimate_learning_rate_in_each_iteration;
    bool        estimate_learning_rate_once;
    bool        return_best_parameters_and_value;
} af_regular_step_gradient_descent_optimizer;

/**
 Gradient Descent Parameters

 \ingroup optimizers_img
 */
typedef struct {
    //TODO: Verify once more; Don't think there are any special attributes
    double      learning_rate;
    unsigned    number_of_iterations;
} af_gradient_descent_optimizer;

/**
 Generic optimizer structure

 This objects wraps a specific optimizer object

 \ingroup optimizers_img
 */
typedef struct {
    af_optimizer_type type;

    /// optimizer should point to any one of the above
    /// af_*_optimizer structures
    void* optimizer;
} af_optimizer;
#endif

#ifdef __cplusplus
}
#endif
