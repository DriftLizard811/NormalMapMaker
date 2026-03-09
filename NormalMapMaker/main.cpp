#include <cmath>
#include <fstream>
#include <iostream>
#include <string>

#include "Color.h"
#include "Image.h"
#include "TextureSet.h"
#include "MathLib.h"

int main()
{    
    std::string filename;
    std::ifstream fin;   
    std::ofstream fout;
    int width;
    int height;
    int pixelFormat;
    std::string trash;

    //until nothing entered
    do
    {
        //get the name of the file to attempt to open
        std::cout << "filename: ";
        std::getline(std::cin, filename);

        //attempt to open the file
        fin.open(filename);
        if (!fin.fail())
        {
            //throwaway header lines, but ensure proper format
            std::getline(fin, trash);
            std::cout << trash;
            if (trash == REQUIRED_FORMAT)
            {
                //throwaway gimp line
                std::getline(fin, trash);

                //get the size of the image
                fin >> width;
                fin >> height;
                std::cout << std::endl << "Width: " << width << ", Height: " << height;
                
                //get the pixel format
                fin >> pixelFormat;
                std::cout << std::endl << "Pixel Format: " << pixelFormat << std::endl;

                //create input and output images
                Image inputImage = Image(width, height, pixelFormat);
                Image outputImage = Image(width, height, pixelFormat);

                //populate the input array pixel by pixel
                inputImage.readFromFile(fin);

                //evaluate pixels
                float above, below, averageVertical;
                float left, right, averageHorizontal;
                int r, g, b;
                for (int i = 0; i < height; i++)
                {
                    for (int j = 0; j < width; j++)
                    {
                        //evalutate left-right curves
                        if (j == width - 1)
                        {
                            left = (float)(inputImage.getPixelValue(j - 1, i).getR() - inputImage.getPixelValue(j, i).getR() + pixelFormat) / (float)(2 * pixelFormat);
                            right = (float)(inputImage.getPixelValue(j, i).getR() - inputImage.getPixelValue(0, i).getR() + pixelFormat) / (float)(2 * pixelFormat);
                            averageHorizontal = (left + right) / 2.0f;
                        }
                        else if (j == 0)
                        {
                            left = (float)(inputImage.getPixelValue(width - 1, i).getR() - inputImage.getPixelValue(j + 1, i).getR() + pixelFormat) / (float)(2 * pixelFormat);
                            right = (float)(inputImage.getPixelValue(j, i).getR() - inputImage.getPixelValue(j + 1, i).getR() + pixelFormat) / (float)(2 * pixelFormat);
                            averageHorizontal = (left + right) / 2.0f;
                        }
                        else
                        {
                            left = averageHorizontal = (float)(inputImage.getPixelValue(j - 1, i).getR() - inputImage.getPixelValue(j, i).getR() + pixelFormat) / (float)(2 * pixelFormat);
                            right = averageHorizontal = (float)(inputImage.getPixelValue(j, i).getR() - inputImage.getPixelValue(j + 1, i).getR() + pixelFormat) / (float)(2 * pixelFormat);
                            averageHorizontal = (left + right) / 2.0f;
                        }

                        //evaluate up-down curves
                        if (i == height - 1)
                        {
                            above = (float)(inputImage.getPixelValue(j, i).getR() - inputImage.getPixelValue(j, i - 1).getR() + pixelFormat) / (float)(2 * pixelFormat);
                            below = (float)(inputImage.getPixelValue(j, 0).getR() - inputImage.getPixelValue(j, i).getR() + pixelFormat) / (float)(2 * pixelFormat);
                            averageVertical = (above + below) / 2.0f;
                        }
                        else if (i == 0)
                        {
                            above = (float)(inputImage.getPixelValue(j, i).getR() - inputImage.getPixelValue(j, height - 1).getR() + pixelFormat) / (float)(2 * pixelFormat);
                            below = (float)(inputImage.getPixelValue(j, i + 1).getR() - inputImage.getPixelValue(j, i).getR() + pixelFormat) / (float)(2 * pixelFormat);
                            averageVertical = (above + below) / 2.0f;
                        }
                        else
                        {
                            above = (float)(inputImage.getPixelValue(j, i).getR() - inputImage.getPixelValue(j, i - 1).getR() + pixelFormat) / (float)(2 * pixelFormat);
                            below = (float)(inputImage.getPixelValue(j, i + 1).getR() - inputImage.getPixelValue(j, i).getR() + pixelFormat) / (float)(2 * pixelFormat);
                            averageVertical = (above + below) / 2.0f;
                        }

                        r = averageHorizontal * pixelFormat;
                        g = averageVertical * pixelFormat;
                        float horizontalDiff = std::abs(0.5f - averageHorizontal);
                        float verticalDiff = std::abs(0.5f - averageVertical);
                        float diffFactor = 1.0f - ((horizontalDiff + verticalDiff));
                        b = diffFactor * pixelFormat;
                        outputImage.setPixelValue(Color(r, g, b), j, i);
                    }
                }

                //output the resulting image
                fout.open("output\\output.ppm");                
                outputImage.writeToFile(fout);
                fout.close();
            }            
        }

        //close the file
        fin.close();
    } while (filename != "q" && filename != "Q");
    
    std::cout << "Program ended";
}