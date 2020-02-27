/*******************************************************
 * Copyright (c) 2020, ArrayFire
 * All rights reserved.
 *
 * This file is distributed under 3-clause BSD license.
 * The complete license agreement can be obtained at:
 * http://arrayfire.com/licenses/BSD-3-Clause
 ********************************************************/

#include <cstdio>
#include <cstring>
#include <arrayfire.h>

using namespace af;

int main(int argc, char* argv[])
{
    int device = argc > 1 ? atoi(argv[1]) : 0;
    try {
        af::setDevice(device);
        af::info();

        // seconds to run demo
        double time_total = 10;

        // initialize parameters
        unsigned particles = 10000;
        float revolutions = 5.0;
        float xAx = 0.7f;
        float yAx = 0.3f;
        float zD = -0.0025f;

        float zero = 0;

        array Z = randu(particles);
        array T = randu(particles) * 2 * af::Pi;

        array X = xAx * Z * cos(T) + 0.5;
        array Y = yAx * Z * sin(T) + 0.5;

        unsigned iter = 0;

        af::Window simWind(1920, 1080, "Simulation of a vortex of particles");

        while (!simWind.close()) {
            Z = Z + zD;

            // Remove old points and add new points
            Z = array(Z < zero) + Z * array(Z >= zero);

            // Update the X and Y points
            array TEMP = T + 2 * af::Pi * revolutions * .00025 * (iter++);
            X = xAx * Z * cos(TEMP) + 0.5;
            Y = yAx * Z * sin(TEMP) + 0.5;

            simWind.scatter(X, Y, Z, AF_MARKER_CROSS);
        }
    } catch (af::exception& e) {
        fprintf(stderr, "%s\n", e.what());
        throw;
    }
    return 0;
}
