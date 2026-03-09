#include <cmath>
#include <fstream>
#include <iostream>
#include <string>

#include "Color.h"
#include "Image.h"
#include "TextureSet.h"
#include "MathLib.h"

const std::string DEFAULT_DESTINATION = "output";
const std::string OUTPUT_FOLDER = "output\\";

int main()
{    
    std::string filename;
    std::string destination;
    TextureSet textureSet;

    //until nothing entered
    do
    {
        //get the name of the file to attempt to open
        std::cout << "filename: ";
        std::getline(std::cin, filename);
        std::cout << "destination: ";
        std::getline(std::cin, destination);

        //try to load the file as a normal map, and only proceed if successful
        if (textureSet.loadDisplacementMapFromFile(filename))
        {
            //create the normal map
            textureSet.generateNormalFromDisplacement();

            //check what destination to use
            if (destination == "")
            {
                textureSet.getNormalMap().writeToFileAsPPM(OUTPUT_FOLDER + DEFAULT_DESTINATION);
            }
            else
            {
                textureSet.getNormalMap().writeToFileAsPPM(OUTPUT_FOLDER + destination);
            }

            //cleanup the set so that it can be reused
            textureSet.cleanup();
        }        
    } while (filename != "q" && filename != "Q");
    
    std::cout << "Program ended";
}