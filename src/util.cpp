/*
 *   Author: Carlos Henrique Silva Correia de Araujo
 *   UFPB Computer engineering student
 *   https://github.com/ch94ca
 *
 */

#include <opencv2/core.hpp>

static cv::Scalar randomColor(cv::RNG& rng)
{
        int icolor = (unsigned)rng;
        return cv::Scalar(icolor&255, (icolor>>8)&255, (icolor>>16)&255);

} // end randomColor
