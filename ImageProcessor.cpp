/* Artyom Kushnir st135665@student.spbu.ru
*/
#include "ImageProcessor.h"
#include <iostream>
#include <fstream>

void ImageProcessor::loadImage(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);

    unsigned char header[54];
    file.read(reinterpret_cast<char*>(header), sizeof(header));

    int width = *reinterpret_cast<int*>(&header[18]);
    int height = *reinterpret_cast<int*>(&header[22]);

    image.resize(height, std::vector<Pixel>(width));

    for (int y = height - 1; y >= 0; --y) { 
        for (int x = 0; x < width; ++x) {
            unsigned char gray;
            file.read(reinterpret_cast<char*>(&gray), sizeof(gray));
            image[y][x] = { gray };
        }
    }

    file.close();
}

void ImageProcessor::printMemoryUsage() const {
    size_t memoryUsed = sizeof(Pixel) * image.size() * image[0].size();
    std::cout << "Memory used: " << memoryUsed << " bytes" << std::endl;
}

std::vector<std::vector<Pixel>> ImageProcessor::rotateClockwise() const {
    int width = image[0].size();
    int height = image.size();
    std::vector<std::vector<Pixel>> rotated(width, std::vector<Pixel>(height));

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            rotated[x][height - 1 - y] = image[y][x];
        }
    }
    
    return rotated;
}

std::vector<std::vector<Pixel>> ImageProcessor::rotateCounterclockwise() const {
    int width = image[0].size();
    int height = image.size();
    std::vector<std::vector<Pixel>> rotated(width, std::vector<Pixel>(height));

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            rotated[width - 1 - x][y] = image[y][x];
        }
    }
    
    return rotated;
}

void ImageProcessor::saveBMP(const std::vector<std::vector<Pixel>>& img, const std::string& filename) const {
    int width = img[0].size();
    int height = img.size();
    
    unsigned char bmpFileHeader[14] = {
        'B', 'M',
        0, 0, 0, 0,
        0, 0,
        0, 0,
        54, 0, 0, 0
    };

    unsigned char bmpInfoHeader[40] = {
        40, 0, 0, 0,
        static_cast<unsigned char>(width & 0xFF), static_cast<unsigned char>((width >> 8) & 0xFF),
        static_cast<unsigned char>((width >> 16) & 0xFF), static_cast<unsigned char>((width >> 24) & 0xFF),
        static_cast<unsigned char>(height & 0xFF), static_cast<unsigned char>((height >> 8) & 0xFF),
        static_cast<unsigned char>((height >> 16) & 0xFF), static_cast<unsigned char>((height >> 24) & 0xFF),
        1, 0,
        8, 0,
        0, 0, 0, 0,
        static_cast<unsigned char>(2835 & 0xFF), static_cast<unsigned char>((2835 >> 8) & 0xFF),
        static_cast<unsigned char>((2835 >> 16) & 0xFF), static_cast<unsigned char>((2835 >> 24) & 0xFF),
        static_cast<unsigned char>(2835 & 0xFF), static_cast<unsigned char>((2835 >> 8) & 0xFF),
        static_cast<unsigned char>((2835 >> 16) & 0xFF), static_cast<unsigned char>((2835 >> 24) & 0xFF),
        0, 0, 0, 0,
        0, 0, 0, 0
    };

    int rowSize = (width + 3) & (~3); 
    int imageSize = rowSize * height;

    bmpFileHeader[2] = static_cast<unsigned char>(imageSize + sizeof(bmpFileHeader) + sizeof(bmpInfoHeader));
    
    std::ofstream file(filename, std::ios::out | std::ios::binary);

    file.write(reinterpret_cast<char*>(bmpFileHeader), sizeof(bmpFileHeader));
    file.write(reinterpret_cast<char*>(bmpInfoHeader), sizeof(bmpInfoHeader));

    for (int y = height - 1; y >= 0; --y) { 
        for (int x = 0; x < width; ++x) {
            const Pixel& pixel = img[y][x]; 
            file.put(pixel.gray); 
        }

        for (int padding = rowSize - width; padding > 0; --padding) {
            file.put(0);
        }
    }

    file.close();
}

