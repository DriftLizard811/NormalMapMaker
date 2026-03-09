#include "Image.h"

Image::Image()
{

}

Image::Image(int width, int height, PixelFormat pixelFormat)
{
    //call init with given specs
    init(width, height, pixelFormat);
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

    //null the pointer
    mpImagePixels = nullptr;

    //flag as uninitialized
    mIsInitted = false;
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

void Image::init(int width, int height, PixelFormat pixelFormat)
{
    //cleanup if already initialized
    if (mIsInitted) cleanup();

    //flag as initialized
    mIsInitted = true;

    //store size and other metrics
    mHeight = height;
    mWidth = width;
    mPixelFormat = pixelFormat;

    //create a blank image
    mpImagePixels = new Color * [mWidth];
    for (int i = 0; i < mWidth; i++)
    {
        mpImagePixels[i] = new Color[mHeight];
    }
}

void Image::readFromFileAsPPM(std::ifstream& fin)
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

    //flag as initialized
    mIsInitted = true;
}

bool Image::readFromFileAsPPM(std::string filename)
{
    //generate the full filename
    std::string fullFilename = filename + PPM_FILENAME_EXTENSION;

    //attempt to open the file
    std::ifstream fin;
    fin.open(fullFilename);

    //check if file is valid
    if (!fin.fail())
    {
        //ensure that format is proper
        std::string readFormat;
        std::getline(fin, readFormat);
        if (readFormat == PPM_REQUIRED_FORMAT)
        {
            //throwaway gimp line
            std::getline(fin, readFormat);
            
            //read image metrics
            fin >> mWidth;
            fin >> mHeight;
            int readPixelFormat;
            fin >> readPixelFormat;
            if (readPixelFormat == PPM_255_FORMAT_SIZE)
            {
                mPixelFormat = PPM_255;
                std::cout << "Image of type .ppm read from file" << std::endl
                          << "Size: " << mWidth << "x" << mHeight << std::endl
                          << "Pixel Format: 255" << std::endl;

                //if the image hasn't been initialized yet, do so according to the specs
                init(mWidth, mHeight, PPM_255);
            }

            //call full read method and then close the file
            readFromFileAsPPM(fin);
            fin.close();

            return true;
        }
        else
        {
            return false;
        }        
    }
    else
    {
        return false;
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

void Image::writeToFileAsPPM(std::string filename)
{
    //create the output stream
    std::ofstream fout;
    fout.open(filename + PPM_FILENAME_EXTENSION);

    //write the header
    fout << PPM_REQUIRED_FORMAT << std::endl
         << mWidth << " " << mHeight << std::endl
         << PPM_255_FORMAT_SIZE << std::endl;

    //output each pixel
    for (int i = 0; i < mHeight; i++)
    {
        for (int j = 0; j < mWidth; j++)
        {
            getPixelReference(j, i).Output(fout);
        }
    }

    //close the file
    fout.close();
}