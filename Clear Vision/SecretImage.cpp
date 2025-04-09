#include "SecretImage.h"

// Constructor: split image into upper and lower triangular arrays
SecretImage::SecretImage(const GrayscaleImage& image) {
    // Get the dimensions of the image
    width = image.get_width();   // Assign width
    height = image.get_height(); // Assign height

    // Calculate the number of elements in the upper and lower triangular matrices
    int lowerMatriceSize = (width * (width - 1)) / 2; // Excludes diagonal
    int upperMatriceSize = (width * (width + 1)) / 2; // Includes diagonal

    // Dynamically allocate memory for the triangular matrices
    lower_triangular = new int[lowerMatriceSize];
    upper_triangular = new int[upperMatriceSize];

    // Variables to track the index of the lower and upper matrices
    int lowerIndex = 0;
    int upperIndex = 0;

    // Fill the upper and lower matrices from the image
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int pixelValue = image.get_pixel(i, j);

            if (i > j) {
                // Lower triangular (excluding diagonal)
                lower_triangular[lowerIndex++] = pixelValue;
            }

            if (i <= j) {
                // Upper triangular (including diagonal)
                upper_triangular[upperIndex++] = pixelValue;
            }
        }
    }
}

// Constructor: instantiate based on data read from file
SecretImage::SecretImage(int w, int h, int* upper, int* lower) {
    // Store the width and height
    width = w;
    height = h;

    // Calculate the size of the triangular matrices
    int upperMatriceSize = (width * (width + 1)) / 2;   // Including diagonal
    int lowerMatriceSize = (width * (width - 1)) / 2;   // Excluding diagonal

    // Dynamically allocate memory for the triangular arrays
    upper_triangular = new int[upperMatriceSize];
    lower_triangular = new int[lowerMatriceSize];

    // Copy the data from the input arrays to the instance variables
    std::copy(upper, upper + upperMatriceSize, upper_triangular);
    std::copy(lower, lower + lowerMatriceSize, lower_triangular);
}

// Destructor: free the arrays
SecretImage::~SecretImage() {
    delete[] upper_triangular;
    delete[] lower_triangular;
}

// Reconstructs and returns the full image from upper and lower triangular matrices.
GrayscaleImage SecretImage::reconstruct() const {
    GrayscaleImage image(width, height);

    // Iterate through each pixel of the image
    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            if (col >= row) {
                // Upper triangular matrix including diagonal (i <= j)
                int index = (row * width) - (row * (row - 1)) / 2 + (col - row);
                image.set_pixel(row, col, upper_triangular[index]);
            }
            else {
                // Lower triangular matrix excluding diagonal (i > j)
                int index = (row * (row - 1)) / 2 + col;
                image.set_pixel(row, col, lower_triangular[index]);
            }
        }
    }

    return image;
}

// Save the filtered image back to the triangular arrays
void SecretImage::save_back(const GrayscaleImage& image) {
    // Update the lower and upper triangular matrices 
    // based on the GrayscaleImage given as the parameter.

    int lowerIndex = 0;
    int upperIndex = 0;

    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            int pixelValue = image.get_pixel(row, col);

            if (row > col) {
                lower_triangular[lowerIndex++] = pixelValue;
            }
            else {
                upper_triangular[upperIndex++] = pixelValue;
            }
        }
    }
}

// Save the upper and lower triangular arrays to a file
void SecretImage::save_to_file(const std::string& filename) {
    // Open the file in output mode
    std::ofstream file(filename);

    // Check if the file is open
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file for writing.");
    }

    // Write width and height on the first line
    file << width << ' ' << height << '\n';

    // Write the upper triangular matrix (size = width * (width + 1) / 2)
    int upperSize = (width * (width + 1)) / 2;
    for (int i = 0; i < upperSize; ++i) {
        file << upper_triangular[i];
        if (i < upperSize - 1) {
            file << ' ';  // Add space between values
        }
    }
    file << '\n';  // End the second line

    // Write the lower triangular matrix (size = width * (width - 1) / 2)
    int lowerSize = (width * (width - 1)) / 2;
    for (int i = 0; i < lowerSize; ++i) {
        file << lower_triangular[i];
        if (i < lowerSize - 1) {
            file << ' ';  // Add space between values
        }
    }
    file << '\n';  // End the third line

    // Close the file
    file.close();
}

// Static function to load a SecretImage from a file
SecretImage SecretImage::load_from_file(const std::string& filename) {
    std::ifstream file(filename);

    // Read width and height from the first line
    int w, h;
    file >> w >> h;

    // Calculate the size of the triangular arrays
    int upperSize = (w * (w + 1)) / 2;
    int lowerSize = (w * (w - 1)) / 2;

    // Allocate memory for the arrays
    int* upper_triangular = new int[upperSize];
    int* lower_triangular = new int[lowerSize];

    // Read the upper triangular array from the second line
    for (int i = 0; i < upperSize; ++i) {
        file >> upper_triangular[i];
    }

    // Read the lower triangular array from the third line
    for (int i = 0; i < lowerSize; ++i) {
        file >> lower_triangular[i];
    }

    file.close();

    // Return a properly initialized SecretImage object
    return SecretImage(w, h, upper_triangular, lower_triangular);
}

// Returns a pointer to the upper triangular part of the secret image.
int* SecretImage::get_upper_triangular() const {
    return upper_triangular;
}

// Returns a pointer to the lower triangular part of the secret image.
int* SecretImage::get_lower_triangular() const {
    return lower_triangular;
}

// Returns the width of the secret image.
int SecretImage::get_width() const {
    return width;
}

// Returns the height of the secret image.
int SecretImage::get_height() const {
    return height;
}
