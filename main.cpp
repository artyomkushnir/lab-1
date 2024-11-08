/* Artyom Kushnir st135665@student.spbu.ru
*/
#include <iostream>
#include <vector>
#include "ImageProcessor.h"
#include "GaussianFilter.h"

int main() {
    std::string inputFilename = "input.bmp";
    std::string outputRotatedClockwise = "rotated_clockwise.bmp";
    std::string outputRotatedCounterclockwise = "rotated_counterclockwise.bmp";
    std::string outputFiltered = "filtered.bmp";

    ImageProcessor imgProcessor;

    try {
        imgProcessor.loadImage(inputFilename);
        imgProcessor.printMemoryUsage();

        auto rotatedClockwise = imgProcessor.rotateClockwise();
        imgProcessor.saveBMP(rotatedClockwise, outputRotatedClockwise);

        auto rotatedCounterclockwise = imgProcessor.rotateCounterclockwise();
        imgProcessor.saveBMP(rotatedCounterclockwise, outputRotatedCounterclockwise);

        auto filteredImage = applyGaussianFilter(rotatedClockwise);
        imgProcessor.saveBMP(filteredImage, outputFiltered);

    } catch (const std::exception &e) {
        std::cerr << "Error:" << e.what() << std::endl;
        return 1;
    }

    return 0;
}
