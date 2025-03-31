/* Artyom Kushnir st135665@student.spbu.ru
*/

#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <cmath>
#include <utility>
#include <algorithm>

struct Image {
    const int fileHeaderSize = 14;
    const int informationHeaderSize = 40;
    unsigned char fileHeader[14] = {0};
    unsigned char informationHeader[40] = {0};
    int width;
    int height;
    int rowSize;
    std::string path;

    Image(const std::string& p) : width(0), height(0), rowSize(0), path(p) {}

    void readHeaders(const std::string& filePath) {
        std::ifstream file(filePath, std::ios::binary);
        if (!file) {
            throw std::runtime_error("Cannot open file: " + filePath);
        }

        //Read file and information headers
        file.read(reinterpret_cast<char*>(fileHeader), fileHeaderSize);
        file.read(reinterpret_cast<char*>(informationHeader), informationHeaderSize);

        //Extract width and height
        width = *reinterpret_cast<int*>(&informationHeader[4]);
        height = *reinterpret_cast<int*>(&informationHeader[8]);

        // Calculate row size (including padding)
        rowSize = (width * 3 + 3) & ~3;

        file.close();
    }

    std::vector<unsigned char> readToVector(const std::string& filePath) {
        readHeaders(filePath);

        size_t dataSize = rowSize * height;
        std::ifstream file(filePath, std::ios::binary);
        if (!file) {
            throw std::runtime_error("Cannot open file: " + filePath);
        }

        //Skip headers
        file.seekg(fileHeaderSize + informationHeaderSize);

        //Read pixel data
        std::vector<unsigned char> buffer(dataSize);
        file.read(reinterpret_cast<char*>(buffer.data()), dataSize);
        file.close();

        return buffer;
    }

    void writeFromVector(const std::string& outputPath, const std::vector<unsigned char>& data) {
        int dataSize = rowSize * height;
        int fileSize = fileHeaderSize + informationHeaderSize + dataSize;

        //Update file header
        fileHeader[2] = fileSize & 0xFF;
        fileHeader[3] = (fileSize >> 8) & 0xFF;
        fileHeader[4] = (fileSize >> 16) & 0xFF;
        fileHeader[5] = (fileSize >> 24) & 0xFF;

        //Update information header
        informationHeader[4] = width & 0xFF;
        informationHeader[5] = (width >> 8) & 0xFF;
        informationHeader[6] = (width >> 16) & 0xFF;
        informationHeader[7] = (width >> 24) & 0xFF;

        informationHeader[8] = height & 0xFF;
        informationHeader[9] = (height >> 8) & 0xFF;
        informationHeader[10] = (height >> 16) & 0xFF;
        informationHeader[11] = (height >> 24) & 0xFF;

        //Write file
        std::ofstream file(outputPath, std::ios::binary);
        if (!file) {
            throw std::runtime_error("Cannot open file: " + outputPath);
        }

        file.write(reinterpret_cast<const char*>(fileHeader), fileHeaderSize);
        file.write(reinterpret_cast<const char*>(informationHeader), informationHeaderSize);

        //Write pixel data row by row
        for (int y = 0; y < height; ++y) {
            file.write(reinterpret_cast<const char*>(&data[y * rowSize]), rowSize);
        }

        file.close();
    }

    void rotate90Clockwise(std::vector<unsigned char>& data) {
        std::vector<unsigned char> rotated(width * height * 3);

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                for (int c = 0; c < 3; ++c) {
                    rotated[((width - x - 1) * height +  y) * 3 + c] =
                        data[y * rowSize + x * 3 + c];
                }
            }
        }

        std::swap(width, height);
        rowSize = (width * 3 + 3) & ~3;
        data = std::move(rotated);
    }

    void rotate90CounterClockwise(std::vector<unsigned char>& data) {
        std::vector<unsigned char> rotated(width * height * 3);

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                for (int c = 0; c < 3; ++c) {
                    rotated[(x * height +  (height - y - 1)) * 3 + c] =
                        data[y * rowSize + x * 3 + c];
                }
            }
        }

        std::swap(width, height);
        rowSize = (width * 3 + 3) & ~3;
        data = std::move(rotated);
    }

    void applyGaussianFilter(std::vector<unsigned char>& data) {
        const int kernelSize = 3;
        const float kernel[kernelSize][kernelSize] = {
            {1.0f / 16, 2.0f / 16, 1.0f / 16},
            {2.0f / 16, 4.0f / 16, 2.0f / 16},
            {1.0f / 16, 2.0f / 16, 1.0f / 16}
        };

        std::vector<unsigned char> filtered(rowSize * height);


        for (int y = 1; y < height - 1; ++y) {
            for (int x = 1; x < width - 1; ++x) {
                for (int c = 0; c < 3; ++c) {
                    float sum = 0.0f;
                    for (int ky = 0; ky < kernelSize; ++ky) {
                        for (int kx = 0; kx < kernelSize; ++kx) {
                            int px = x + kx - 1;
                            int py = y + ky - 1;
                            sum += kernel[ky][kx] * data[py * rowSize + px * 3 + c];
                        }
                    }
                    filtered[y * rowSize + x * 3 + c] = static_cast<unsigned char>(std::max(0.0f, std::min(255.0f, sum)));
                }
            }
        }

        data = std::move(filtered);
    }

};

#endif