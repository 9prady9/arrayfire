/*******************************************************
 * Copyright (c) 2017, ArrayFire
 * All rights reserved.
 *
 * This file is distributed under 3-clause BSD license.
 * The complete license agreement can be obtained at:
 * http://arrayfire.com/licenses/BSD-3-Clause
 ********************************************************/

#include <af/dim4.hpp>
#include <af/defines.h>
#include <af/image_registration.h>
#include <err_common.hpp>
#include <backend.hpp>
#include <handle.hpp>

#include <utility>

using af::dim4;
using namespace detail;

template<typename T>
af_array registerImage(
        unsigned* its,                      //iterations_taken,
        double* fmv,                        //final_metric_value,
        const af_array fimg,                //fixed_image,
        const af_array mimg,                //moving_image,
        const af_array it,                  //initial_transform,
        const af_array ift,                 //initial_fixed_transform,
        const af_array imt,                 //initial_moving_transform,
        const af_registration_method* mtd,  //method,
        const af_optimizer* opt,            //optimizer,
        const af_metric* mtr,               //metric,
        const unsigned numl,                //num_levels,
        const float* sigs,                  //smoothing_sigmas,
        const float* facs                   //shrink_factors
        )
{
    //TODO call backend implementations
    return mimg;
}

af_err af_register_image_translation_transform(
        af_array* out,
        unsigned* its,                      //iterations_taken,
        double* fmv,                        //final_metric_value,
        const af_array fimg,                //fixed_image,
        const af_array mimg,                //moving_image,
        const af_array it,                  //initial_transform,
        const af_array ift,                 //initial_fixed_transform,
        const af_array imt,                 //initial_moving_transform,
        const af_registration_method* mtd,  //method,
        const af_optimizer* opt,            //optimizer,
        const af_metric* mtr,               //metric,
        const unsigned numl,                //num_levels,
        const float* sigs,                  //smoothing_sigmas,
        const float* facs                   //shrink_factors
        )
{
    try {
        const ArrayInfo& fInfo = getInfo(fimg);

        //TODO input validation checks

        af_array output;

        af_dtype type  = fInfo.getType();
        switch(type) {
            case f32: output = registerImage<float >(its, fmv, fimg, mimg, it, ift, imt, mtd, opt, mtr, numl, sigs, facs); break;
            case f64: output = registerImage<double>(its, fmv, fimg, mimg, it, ift, imt, mtd, opt, mtr, numl, sigs, facs); break;
            case s32: output = registerImage<int   >(its, fmv, fimg, mimg, it, ift, imt, mtd, opt, mtr, numl, sigs, facs); break;
            case u32: output = registerImage<uint  >(its, fmv, fimg, mimg, it, ift, imt, mtd, opt, mtr, numl, sigs, facs); break;
            case s16: output = registerImage<short >(its, fmv, fimg, mimg, it, ift, imt, mtd, opt, mtr, numl, sigs, facs); break;
            case u16: output = registerImage<ushort>(its, fmv, fimg, mimg, it, ift, imt, mtd, opt, mtr, numl, sigs, facs); break;
            case u8:  output = registerImage<uchar >(its, fmv, fimg, mimg, it, ift, imt, mtd, opt, mtr, numl, sigs, facs); break;
            default : TYPE_ERROR(1, type);
        }
        // output array is binary array
        std::swap(output, *out);
    }
    CATCHALL;

    return AF_SUCCESS;
}
