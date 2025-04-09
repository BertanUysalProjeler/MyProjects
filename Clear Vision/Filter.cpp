#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include "Filter.h"
#include <algorithm>
#include <cmath>
#include <vector>
#include <numeric>
#include <math.h>

// Mean Filter
void Filter::apply_mean_filter(GrayscaleImage& image, int kernelSize) {
    // TODO: Your code goes here.
    // 1. Copy the original image for reference.
    // 2. For each pixel, calculate the mean value of its neighbors using a kernel.
    // 3. Update each pixel with the computed mean.

    int height = image.get_height();
    int width = image.get_width();

    // Create a copy of the original image data for reference
    unsigned char** originalData = image.get_data();

    // Create a new image matrix to store the filtered data
    unsigned char** filteredData = new unsigned char* [height];
    for (int i = 0; i < height; ++i) {
        filteredData[i] = new unsigned char[width];
    }

    int kernelOffset = kernelSize / 2;

    // Iterate through each pixel in the image
    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            int sum = 0;
            int count = 0;

            // Iterate through the kernel (with zero-padding for out-of-bound indices)
            for (int i = -kernelOffset; i <= kernelOffset; ++i) {
                for (int j = -kernelOffset; j <= kernelOffset; ++j) {
                    int neighborRow = row + i;
                    int neighborCol = col + j;

                    // Check if the neighbor pixel is within the image bounds
                    if (neighborRow >= 0 && neighborRow < height && neighborCol >= 0 && neighborCol < width) {
                        sum += originalData[neighborRow][neighborCol];
                    }
                    else {
                        // Add 0 for out-of-bound neighbors (zero padding)
                        sum += 0;
                    }
                    count++;
                }
            }

            // Calculate the floored mean value for the current pixel
            int meanValue = static_cast<int>(std::floor(static_cast<double>(sum) / count));

            // Update the pixel in the filtered data
            filteredData[row][col] = static_cast<unsigned char>(meanValue);
        }
    }

    // Replace the original data with the filtered data
    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            image.set_pixel(row, col, filteredData[row][col]);
        }
    }

    // Cleanup: Deallocate the filteredData matrix
    for (int i = 0; i < height; ++i) {
        delete[] filteredData[i];
    }
    delete[] filteredData;
}

// Gaussian Smoothing Filter
void Filter::apply_gaussian_smoothing(GrayscaleImage& image, int kernelSize, double sigma) {
    // TODO: Your code goes here.
    // 1. Create a Gaussian kernel based on the given sigma value.
    // 2. Normalize the kernel to ensure it sums to 1.
    // 3. For each pixel, compute the weighted sum using the kernel.
    // 4. Update the pixel values with the smoothed results.

    int height = image.get_height();
    int width = image.get_width();

    // Initialize the Gaussian kernel
    std::vector<std::vector<double>> kernel(kernelSize, std::vector<double>(kernelSize));
    double sigmaSquared = 2 * sigma * sigma;
    double sum = 0.0;
    int offset = kernelSize / 2;

    // Calculate the Gaussian kernel values
    for (int i = -offset; i <= offset; ++i) {
        for (int j = -offset; j <= offset; ++j) {
            double value = (1.0 / (M_PI * sigmaSquared)) *
                std::exp(-(i * i + j * j) / sigmaSquared);
            kernel[i + offset][j + offset] = value;
            sum += value;
        }
    }

    // Normalize the kernel so that the sum of all elements is 1
    for (int i = 0; i < kernelSize; ++i) {
        for (int j = 0; j < kernelSize; ++j) {
            kernel[i][j] /= sum;
        }
    }

    // Copy the original image data
    unsigned char** originalData = image.get_data();

    // Create a temporary buffer to store the filtered values
    std::vector<std::vector<unsigned char>> tempData(height, std::vector<unsigned char>(width, 0));

    // Iterate through each pixel in the image
    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            double weightedSum = 0.0;

            // Apply the Gaussian kernel to the neighborhood of the pixel
            for (int i = -offset; i <= offset; ++i) {
                for (int j = -offset; j <= offset; ++j) {
                    int neighborRow = row + i;
                    int neighborCol = col + j;

                    // Handle boundary pixels by padding with black (0)
                    if (neighborRow >= 0 && neighborRow < height && neighborCol >= 0 && neighborCol < width) {
                        weightedSum += originalData[neighborRow][neighborCol] * kernel[i + offset][j + offset];
                    }
                }
            }

            // Set the new pixel value in the temporary buffer (floor the result)
            tempData[row][col] = static_cast<int>(std::floor(weightedSum));
        }
    }

    // Update the original image with the smoothed values from tempData
    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            image.set_pixel(row, col, tempData[row][col]);
        }
    }
}

int clamp(int value, int min, int max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

// Unsharp Masking Filter
void Filter::apply_unsharp_mask(GrayscaleImage& image, int kernelSize, double amount) {
    // TODO: Your code goes here.
    // 1. Blur the image using Gaussian smoothing, use the default sigma given in the header.
    // 2. For each pixel, apply the unsharp mask formula: original + amount * (original - blurred).
    // 3. Clip values to ensure they are within a valid range [0-255].

    int height = image.get_height();
    int width = image.get_width();

    // Copy the original image to preserve it for the unsharp mask calculation
    GrayscaleImage copiedImage = GrayscaleImage(image);

    // Apply Gaussian smoothing to the copied image
    apply_gaussian_smoothing(copiedImage, kernelSize);

    // Iterate through each pixel in the image
    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            // Use floating-point precision for the unsharp mask calculation
            double original = static_cast<double>(image.get_pixel(row, col));
            double blurred = static_cast<double>(copiedImage.get_pixel(row, col));

            // Calculate the unsharp mask value using floating-point math
            double unsharpMaskedValue = original + amount * (original - blurred);

            // Clamp the result to the range [0, 255] with rounding
            image.set_pixel(row, col, clamp(static_cast<int>(round(unsharpMaskedValue)), 0, 255));
        }
    }
}