/*******************************************************
 * Copyright (c) 2017, ArrayFire
 * All rights reserved.
 *
 * This file is distributed under 3-clause BSD license.
 * The complete license agreement can be obtained at:
 * http://arrayfire.com/licenses/BSD-3-Clause
 ********************************************************/

#pragma once
#include <af/image_registration_metrics.h>
#include <af/image_registration_optimizers.h>
#include <af/image_registration_methods.h>

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
//TODO: Verify API once more
/**
    \param[out] out is the vector mapping translation of reference image to match target image
    \param[in] fixed_image is the target image
    \param[in] moving_image is the image to be transformed in order to be superimposed to the target
    \param[in] initial_transform is the starting transformation to be optimized
    \param[in] initial_fixed_transform is initial transform for \p fixed_image
    \param[in] initial_moving_transform is initial transform for \p moving_image
    \param[in] method has the details of the registration method(\ref af_registration_method) to be used
    \param[in] optimizer has the details of the optimization technique(\ref af_optimizer) to be used
    \param[in] metric has the details of the error metric(\ref af_metric) to be used
    \param[in] num_levels is the number of scale-space levels
    \param[in] smoothing_sigmas has the smoothing sigma to be used for generating an image at that level.
    \param[in] shrink_factors has the shrink factor for generating an image at that level.
    \return    \ref AF_SUCCESS if the image has been registered successfully,
    otherwise an appropriate error code is returned.

    \note \p smoothing_sigmas & \p shrink_factors should be arrays of length \p num_levels

    \ingroup img_registration
 */
AFAPI af_err af_register_image_translation_transform(
        af_array* out,
        unsigned* iterations_taken,
        double* final_metric_value,
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
        );
#endif

#ifdef __cplusplus
}
#endif
