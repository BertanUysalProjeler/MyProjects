#include "GrayscaleImage.h"
#include <iostream>
#include <cstring>  // For memcpy
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <stdexcept>


// Constructor: load from a file
GrayscaleImage::GrayscaleImage(const char* filename) {

    // Image loading code using stbi
    int channels;
    unsigned char* image = stbi_load(filename, &width, &height, &channels, STBI_grey);

    if (image == nullptr) {
        std::cerr << "Error: Could not load image " << filename << std::endl;
        exit(1);
    }

    // TODO: Your code goes here.
    // Dynamically allocate memory for a 2D matrix based on the given dimensions
    // and fill the matrix with pixel values from the image

    // Dynamically allocate memory for a 2D matrix based on the given dimensions
    data = new unsigned char* [height];
    for (int i = 0; i < height; ++i) {
        data[i] = new unsigned char[width];
    }

    // Copy pixel values from the loaded image into the data matrix
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            data[y][x] = image[y * width + x];  // Single channel (grayscale)
        }
    }

    // Free the dynamically allocated memory of stbi image
    stbi_image_free(image);
}

// Constructor: initialize from a pre-existing data matrix
GrayscaleImage::GrayscaleImage(int** inputData, int h, int w) : width(w), height(h) {
    // Dynamically allocate memory for the 2D data array
    data = new unsigned char* [height];
    for (int i = 0; i < height; ++i) {
        data[i] = new unsigned char[width];
    }

    // Copy data from inputData to the data matrix
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            data[y][x] = static_cast<unsigned char>(inputData[y][x]);  // Convert int to unsigned char
        }
    }
}

// Constructor to create a blank image of given width and height
GrayscaleImage::GrayscaleImage(int w, int h) : width(w), height(h) {
    // TODO: Your code goes here.
    // Just dynamically allocate the memory for the new matrix.

    data = new unsigned char* [height];
    for (int i = 0; i < height; ++i) {
        data[i] = new unsigned char[width];
        std::memset(data[i], 0, width);  // Initialize all pixels to 0
    }
}

// Copy constructor
GrayscaleImage::GrayscaleImage(const GrayscaleImage& other) : width(other.width), height(other.height) {
    // TODO: Your code goes here.
    // Copy constructor: dynamically allocate memory and 
      // copy pixel values from another image.

    // Dynamically allocate memory and copy pixel values from the other image
    data = new unsigned char* [height];
    for (int i = 0; i < height; ++i) {
        data[i] = new unsigned char[width];
        std::memcpy(data[i], other.data[i], width);  // Copy pixel data
    }
}

// Destructor
GrayscaleImage::~GrayscaleImage() {
    // TODO: Your code goes here.
    // Destructor: deallocate memory for the matrix.

    // Free the dynamically allocated memory
    for (int i = 0; i < height; ++i) {
        delete[] data[i];
    }
    delete[] data;
}

// Equality operator
bool GrayscaleImage::operator==(const GrayscaleImage& other) const {
    // TODO: Your code goes here.
    // Check if two images have the same dimensions and pixel values.
    // If they do, return true.

    // Check if dimensions are the same
    if (width != other.width || height != other.height) {
        std::cout << "Images have different dimensions.\n";
        return false;
    }

    // Variable to track if images are identical
    bool areIdentical = true;

    // Check pixel values
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (data[i][j] != other.data[i][j]) {
                areIdentical = false;
            }
        }
    }

    // Return true if no differences were found, otherwise false
    return areIdentical;
}

// Addition operator
GrayscaleImage GrayscaleImage::operator+(const GrayscaleImage& other) const {
    // TODO: Your code goes here.
    // Add two images' pixel values and return a new image, clamping the results.

    GrayscaleImage result(width, height);
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int sum = data[i][j] + other.data[i][j];
            result.data[i][j] = (sum > 255) ? 255 : sum;  // Clamp to 255
        }
    }
    return result;
}

// Subtraction operator
GrayscaleImage GrayscaleImage::operator-(const GrayscaleImage& other) const {
    // TODO: Your code goes here.
    // Subtract pixel values of two images and return a new image, clamping the results.

    GrayscaleImage result(width, height);
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int diff = data[i][j] - other.data[i][j];
            result.data[i][j] = (diff < 0) ? 0 : diff;  // Clamp to 0
        }
    }
    return result;
}

// Get a specific pixel value
int GrayscaleImage::get_pixel(int row, int col) const {
    return data[row][col];
}

// Set a specific pixel value
void GrayscaleImage::set_pixel(int row, int col, int value) {
    data[row][col] = value;
}

// Function to save the image to a PNG file
void GrayscaleImage::save_to_file(const char* filename) const {
    unsigned char* imageBuffer = new unsigned char[width * height];

    // Fill the buffer with pixel data (convert int to unsigned char)
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            imageBuffer[i * width + j] = data[i][j];
        }
    }

    // Write the buffer to a PNG file
    if (!stbi_write_png(filename, width, height, 1, imageBuffer, width)) {
        std::cerr << "Error: Could not save image to file " << filename << std::endl;
    }

    // Clean up the allocated buffer
    delete[] imageBuffer;
}
