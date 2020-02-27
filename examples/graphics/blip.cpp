/*******************************************************
 * Copyright (c) 2020, ArrayFire
 * All rights reserved.
 *
 * This file is distributed under 3-clause BSD license.
 * The complete license agreement can be obtained at:
 * http://arrayfire.com/licenses/BSD-3-Clause
 ********************************************************/

#include <cstring>
#include <cstdio>
#include <cmath>
#include <arrayfire.h>

#include <iostream>

using namespace af;

array blip(array& X, array& Y, float sigma, float phi)
{
    array XX = X * X, YY = Y*Y;
    float sigma2 = sigma * sigma, sigma4 = sigma2 * sigma2;
    return -((XX + YY - 2 * sigma2) / sigma4) *
        exp( (-XX + -YY) / (2 * sigma2) ) +
        cos(sqrt(XX + YY) * 3 + phi) / 10;
}

int main(int argc, char *argv[])
{
    int device = argc > 1 ? atoi(argv[1]) : 0;
    try {
        af::setDevice(device);
        af::info();

        double time_total = 10;

        // blip parameters
        float mn = -10, mx = 10;
        int N = 100;
        array v = af::seq(mn, mx, (mx - mn) / N);
        array X = tile(moddims(v,N,1),1,N);
        array Y = tile(moddims(v,1,N),N,1);
        array R = iota(dim4(N));

        float ts = 0;

        af::Window simWind(512, 512, "Blip");

        simWind.setColorMap(AF_COLORMAP_HEAT);

        while (!simWind.close()) {
            array B = blip(X, Y, 2.0f + cos(ts), ts);
            array colB = join(1, R, B(span, 0));
            array colH = join(1, R, B(span, N/2));

            simWind.grid(2,2);

            simWind(0, 0).setAxesLimits(0, N, 0, N, -0.4, 2.0);
            simWind(1, 0).setAxesLimits(0, N, -0.1, 0.1);
            simWind(0, 1).setAxesLimits(0, N, -0.4, 2.0);

            simWind(0, 0).surface(B);
            simWind(1, 0).plot(colB);
            simWind(0, 1).plot(colH);
            simWind(1, 1).image(B);

            simWind.show();

            ts += 0.01f;
        }

    } catch (af::exception& e) {
        fprintf(stderr, "%s\n", e.what());
        throw;
    }

    return 0;
}
