/*******************************************************
 * Copyright (c) 2017, ArrayFire
 * All rights reserved.
 *
 * This file is distributed under 3-clause BSD license.
 * The complete license agreement can be obtained at:
 * http://arrayfire.com/licenses/BSD-3-Clause
 ********************************************************/

#include <af/array.h>
#include <af/image_registration.h>
#include "symbol_manager.hpp"

af_err af_register_image_translation_transform(
        af_array* out,
        const af_array fixed_image,
        const af_array moving_image,
        const af_array initial_transform,
        const af_array initial_fixed_transform,
        const af_array initial_moving_transform,
        const af_registration_method* method,
        const af_optimizer* optimizer,
        const af_metric* metric,
        const unsigned num_levels,
        const float* smoothing_sigmas,
        const float* shrink_factors
        )
{
    CHECK_ARRAYS(fixed_image, moving_image);
    CHECK_ARRAYS(initial_transform, initial_fixed_transform, initial_moving_transform);

    return CALL(out, fixed_image, moving_image, initial_transform,
                initial_fixed_transform, initial_moving_transform,
                method, optimizer, metric, num_levels, smoothing_sigmas, shrink_factors);
}
