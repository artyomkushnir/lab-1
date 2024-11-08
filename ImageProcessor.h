#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <vector>
#include <string>
#include <stdexcept>

struct Pixel {
    unsigned char gray; // Оттенок серого
};

class ImageProcessor {
public:
    void loadImage(const std::string& filename);
    void printMemoryUsage() const;
    std::vector<std::vector<Pixel>> rotateClockwise() const;
    std::vector<std::vector<Pixel>> rotateCounterclockwise() const;
    void saveBMP(const std::vector<std::vector<Pixel>>& image, const std::string& filename) const;

private:
    std::vector<std::vector<Pixel>> image; // Двумерный вектор для хранения изображения
};

#endif // IMAGEPROCESSOR_H
