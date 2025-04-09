#include "Crypto.h"
#include "GrayscaleImage.h"


std::vector<int> Crypto::extract_LSBits(SecretImage& secret_image, int message_length) {
    std::vector<int> LSB_array;

    // Reconstruct the SecretImage to a GrayscaleImage.
    GrayscaleImage image = secret_image.reconstruct();

    // Calculate the image dimensions.
    int width = image.get_width();
    int height = image.get_height();

    // Determine the total bits required based on message length.
    int total_bits = message_length * 7; // each character is 7 bits

    // Ensure the image has enough pixels to hide the message.
    if (width * height < total_bits) {
        throw std::runtime_error("The image is not large enough to contain the secret message.");
    }

    // Extract the LSBs from the image pixels.
    // Iterate from the last pixel backward for the total_bits needed
    for (int i = 0; i < total_bits; i++) {
        // Calculate row and column by starting at the end
        int row = (width * height - total_bits + i) / width;
        int col = (width * height - total_bits + i) % width;

        int pixel_value = image.get_pixel(row, col);
        LSB_array.push_back(pixel_value & 1); // Extract the LSB
    }

    return LSB_array;
}

std::string Crypto::decrypt_message(const std::vector<int>& LSB_array) {
    std::string message;

    // Ensure the LSB array size is a multiple of 7 bits.
    if (LSB_array.size() % 7 != 0) {
        throw std::runtime_error("The LSB array size is not a multiple of 7.");
    }

    // Convert each group of 7 bits into an ASCII character.
    for (size_t i = 0; i < LSB_array.size(); i += 7) {
        std::bitset<7> char_bits;

        // Populate the bitset (ensure reverse order to account for bit extraction)
        for (int j = 0; j < 7; j++) {
            char_bits[6 - j] = LSB_array[i + j]; // Reverse order
        }

        // Convert the bitset to an ASCII character and append to message
        char ascii_char = static_cast<char>(char_bits.to_ulong());
        message.push_back(ascii_char);
    }

    return message;
}

std::vector<int> Crypto::encrypt_message(const std::string& message) {
    std::vector<int> LSB_array;

    // 1. Convert each character of the message into a 7-bit binary representation.
    for (char c : message) {
        std::bitset<7> char_bits(static_cast<unsigned long>(c));

        // 2. Collect the bits into the LSB array, note that bits must be stored in reverse order
        //    (since the LSB is stored first)
        for (int i = 6; i >= 0; i--) {
            LSB_array.push_back(char_bits[i]);
        }
    }

    // 3. Return the array of bits.
    return LSB_array;
}

SecretImage Crypto::embed_LSBits(GrayscaleImage& image, const std::vector<int>& LSB_array) {
    // 1. Get the image dimensions and verify there are enough pixels to store the LSB array.
    int width = image.get_width();
    int height = image.get_height();
    int total_pixels = width * height;
    int lengthOfArray = LSB_array.size();

    if (total_pixels < lengthOfArray) {
        throw std::runtime_error("The image is not large enough to embed the LSB array.");
    }

    // 2. Calculate the starting pixel based on the message length,
    // such that the last LSB is embedded in the last pixel of the image.
    int startIndex = total_pixels - lengthOfArray;

    // 3. Embed the LSB array into the image, starting from the calculated startIndex.
    for (int i = 0; i < lengthOfArray; i++) {
        int row = (startIndex + i) / width;
        int col = (startIndex + i) % width;

        // Get the current pixel value.
        int current_pixel = image.get_pixel(row, col);

        // Embed the LSB if necessary by comparing with the current LSB.
        if (LSB_array[i] != (current_pixel & 1)) {
            // Flip the LSB to match the message bit.
            current_pixel ^= 1;
            image.set_pixel(row, col, current_pixel);
        }
    }

    // 4. Return a SecretImage object constructed from the modified GrayscaleImage.
    SecretImage secret_image(image);
    return secret_image;
}
