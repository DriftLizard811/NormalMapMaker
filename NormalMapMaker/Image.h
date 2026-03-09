#ifndef IMAGE_H
#define IMAGE_H

#include "Color.h"
#include <fstream>
#include <iostream>
#include <string>

const int         PPM_255_FORMAT_SIZE = 255;
const std::string PPM_FILENAME_EXTENSION = ".ppm";
const std::string PPM_REQUIRED_FORMAT = "P3";

enum PixelFormat
{
    PPM_255
};

class Image
{
    public:
        Image();
        Image(int width, int height, PixelFormat pixelFormat);
        ~Image();

        const bool getIsValid() { return mIsInitted; }
        const int getWidth() { return mWidth; }
        const int getHeight() { return mHeight; }
        const PixelFormat getPixelFormat() { return mPixelFormat; }

        void cleanup();
        void init(int width, int height, PixelFormat pixelFormat);
        Color& getPixelReference(int x, int y);
        Color getPixelValue(int x, int y);
        void readFromFileAsPPM(std::ifstream& fin); //assumes that all header info has been cleared
        bool readFromFileAsPPM(std::string filename);
        bool setPixelValue(Color color, int x, int y);
        void writeToFileAsPPM(std::string filename);

    private:
        bool mIsInitted = false;

        Color** mpImagePixels = nullptr; //first dimension = column (x) value, second dimension = row (y) value; STARTS @ TOP LEFT
        int mHeight = 0;
        int mWidth = 0;
        PixelFormat mPixelFormat = PPM_255;
};

#endif