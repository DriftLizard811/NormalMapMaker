#include <cmath>
#include <fstream>
#include <iostream>
#include <string>

#include "Color.h"

const std::string REQUIRED_FORMAT = "P3";

float lerp(float a, float b, float t)
{
    return a + ((b-a) * t);
}

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

                //create a 2D array to store the input colors
                Color** inputArray = new Color*[width];
                Color** outputArray = new Color*[width];
                for (int i = 0; i < width; i++)
                {
                    inputArray[i] = new Color[height];
                    outputArray[i] = new Color[height];
                }

                //populate the input array
                for (int i = 0; i < height; i++)
                {
                    for (int j = 0; j < width; j++)
                    {
                        fin >> inputArray[j][i].R;
                        fin >> inputArray[j][i].G;
                        fin >> inputArray[j][i].B;                        
                    }
                }

                //evaluate pixels
                float above, below, averageVertical;
                float left, right, averageHorizontal;
                for (int i = 0; i < height; i++)
                {
                    for (int j = 0; j < width; j++)
                    {
                        //evalutate left-right curves
                        if (j == width - 1)
                        {
                            left = (float)(inputArray[j - 1][i].R - inputArray[j][i].R + pixelFormat) / (float)(2 * pixelFormat);
                            right = (float)(inputArray[j][i].R - inputArray[0][i].R + pixelFormat) / (float)(2 * pixelFormat);
                            averageHorizontal = (left + right) / 2.0f;
                        }
                        else if (j == 0)
                        {
                            left = (float)(inputArray[width - 1][i].R - inputArray[j + 1][i].R + pixelFormat) / (float)(2 * pixelFormat);
                            right = (float)(inputArray[j][i].R - inputArray[j + 1][i].R + pixelFormat) / (float)(2 * pixelFormat);
                            averageHorizontal = (left + right) / 2.0f;
                        }
                        else
                        {
                            left = averageHorizontal = (float)(inputArray[j - 1][i].R - inputArray[j][i].R + pixelFormat) / (float)(2 * pixelFormat);
                            right = averageHorizontal = (float)(inputArray[j][i].R - inputArray[j + 1][i].R + pixelFormat) / (float)(2 * pixelFormat);
                            averageHorizontal = (left + right) / 2.0f;
                        }

                        //evaluate up-down curves
                        if (i == height - 1)
                        {
                            above = (float)(inputArray[j][i].R - inputArray[j][i - 1].R + pixelFormat) / (float)(2 * pixelFormat);
                            below = (float)(inputArray[j][0].R - inputArray[j][i].R + pixelFormat) / (float)(2 * pixelFormat);
                            averageVertical = (above + below) / 2.0f;
                        }
                        else if (i == 0)
                        {
                            above = (float)(inputArray[j][i].R - inputArray[j][height - 1].R + pixelFormat) / (float)(2 * pixelFormat);
                            below = (float)(inputArray[j][i + 1].R - inputArray[j][i].R + pixelFormat) / (float)(2 * pixelFormat);
                            averageVertical = (above + below) / 2.0f;
                        }
                        else
                        {
                            above = (float)(inputArray[j][i].R - inputArray[j][i - 1].R + pixelFormat) / (float)(2 * pixelFormat);
                            below = (float)(inputArray[j][i + 1].R - inputArray[j][i].R + pixelFormat) / (float)(2 * pixelFormat);
                            averageVertical = (above + below) / 2.0f;
                        }

                        outputArray[j][i].R = averageHorizontal * pixelFormat;
                        outputArray[j][i].G = averageVertical * pixelFormat;
                        float horizontalDiff = std::abs(0.5f - averageHorizontal);
                        float verticalDiff = std::abs(0.5f - averageVertical);
                        float diffFactor = 1.0f - ((horizontalDiff + verticalDiff));
                        outputArray[j][i].B = diffFactor * pixelFormat;
                    }
                }

                //output the resulting image
                //setup header
                fout.open("output.ppm");
                fout << REQUIRED_FORMAT << std::endl
                    << width << " " << height << std::endl
                    << pixelFormat << std::endl;

                //output each pixel
                for (int i = 0; i < height; i++)
                {
                    for (int j = 0; j < width; j++)
                    {
                        outputArray[j][i].Output(fout);
                    }
                }

                fout.close();

                //cleanup memory
                for (int i = 0; i < width; i++)
                {
                    delete inputArray[i];
                    delete outputArray[i];
                }
                delete[] inputArray;
                delete[] outputArray;
            }            
        }

        //close the file
        fin.close();
    } while (filename != "q" && filename != "Q");
    
    std::cout << "Program ended";
}