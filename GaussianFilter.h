#ifndef GAUSSIANFILTER_H
#define GAUSSIANFILTER_H

#include <vector>
#include "ImageProcessor.h"

std::vector<std::vector<Pixel>> applyGaussianFilter(const std::vector<std::vector<Pixel>>& image);

#endif // GAUSSIANFILTER_H
