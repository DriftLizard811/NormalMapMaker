#ifndef IMAGE_H
#define IMAGE_H

#include "Color.h"
#include <fstream>
#include <iostream>

const std::string REQUIRED_FORMAT = "P3";

class Image
{
    public:
        Image();
        Image(int width, int height, int pixelFormat);
        ~Image();

        void cleanup();
        Color& getPixelReference(int x, int y);
        Color getPixelValue(int x, int y);
        void readFromFile(std::ifstream& fin);
        bool setPixelValue(Color color, int x, int y);
        void writeToFile(std::ofstream& fout);

    private:
        Color** mpImagePixels = nullptr; //first dimension = column (x) value, second dimension = row (y) value; STARTS @ TOP LEFT
        int mHeight = 0;
        int mWidth = 0;
        int mPixelFormat = 255;
};

#endif