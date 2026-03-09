#include "Image.h"

Image::Image()
{

}

Image::Image(int width, int height, int pixelFormat)
{
    //store size and other metrics
    mHeight = height;
    mWidth = width;
    mPixelFormat = pixelFormat;

    //create a blank image
    mpImagePixels = new Color*[mWidth];
    for (int i = 0; i < mWidth; i++)
    {
        mpImagePixels[i] = new Color[mHeight];
    }
}

Image::~Image()
{
    cleanup();
}

void Image::cleanup()
{
    //check if there is an image to delete
    if (mpImagePixels != nullptr)
    {
        //delete each column, then the full container
        for (int i = 0; i < mWidth; i++)
        {
            delete mpImagePixels[i];
        }
        delete[] mpImagePixels;
    }
}

Color& Image::getPixelReference(int x, int y)
{
    return mpImagePixels[x][y];
}

Color Image::getPixelValue(int x, int y)
{
    //check within bounds
    if (x >= 0 && x < mWidth && y >= 0 && y < mHeight)
    {
        return mpImagePixels[x][y];
    }
    else
    {
        return Color();
    }
}

void Image::readFromFile(std::ifstream& fin)
{
    //populate the input array pixel by pixel
    Color readPixel;
    int inR, inG, inB;
    for (int i = 0; i < mHeight; i++)
    {
        for (int j = 0; j < mWidth; j++)
        {
            fin >> inR;
            fin >> inG;
            fin >> inB;
            readPixel.setR(inR);
            readPixel.setG(inG);
            readPixel.setB(inB);
            setPixelValue(readPixel, j, i);
        }
    }
}

bool Image::setPixelValue(Color color, int x, int y)
{
    //check within bounds
    if (x >= 0 && x < mWidth && y >= 0 && y < mHeight)
    {
        mpImagePixels[x][y] = color;
        return true;
    }
    else
    {
        return false;
    }
}

void Image::writeToFile(std::ofstream& fout)
{
    fout << REQUIRED_FORMAT << std::endl
         << mWidth << " " << mHeight << std::endl
         << mPixelFormat << std::endl;

    //output each pixel
    for (int i = 0; i < mHeight; i++)
    {
        for (int j = 0; j < mWidth; j++)
        {
            getPixelReference(j, i).Output(fout);
        }
    }
}