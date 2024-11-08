/* Artyom Kushnir st135665@student.spbu.ru
*/
#include "GaussianFilter.h"
#include <array>

std::vector<std::vector<Pixel>> applyGaussianFilter(const std::vector<std::vector<Pixel>>& image) {
    const double kernel[3][3] = {
        {1.0 / 16, 2.0 / 16, 1.0 / 16},
        {2.0 / 16, 4.0 / 16, 2.0 / 16},
        {1.0 / 16, 2.0 / 16, 1.0 / 16}
    };

    int width = image[0].size();
    int height = image.size();
    
    std::vector<std::vector<Pixel>> filtered(height, std::vector<Pixel>(width));

    for (int y = 1; y < height - 1; ++y) {
        for (int x = 1; x < width - 1; ++x) {
            double sum = 0.0;

            for (int ky = -1; ky <= 1; ++ky) {
                for (int kx = -1; kx <= 1; ++kx) {
                    sum += kernel[ky + 1][kx + 1] * image[y + ky][x + kx].gray;
                }
            }

            filtered[y][x].gray = static_cast<unsigned char>(sum);
        }
    }

    return filtered;
}
