#include "TextureSet.h"

TextureSet::TextureSet()
{

}

TextureSet::~TextureSet()
{

}

void TextureSet::cleanup()
{
    mDiffuseMap.cleanup();
    mDisplacementMap.cleanup();
    mNormalMap.cleanup();
}

void TextureSet::generateNormalFromDisplacement()
{
	//check which conversion to run
	switch (mDisplacementMap.getPixelFormat())
	{
		case PPM_255:
			generateNormalFromDisplacement_PPM255();
			break;
		default:
			std::cout << "ERROR: Could not generate a normal map from the given file format" << std::endl << std::endl;
	}
}

void TextureSet::generateNormalFromDisplacement_PPM255()
{
    //store width and height of source
    int width = mDisplacementMap.getWidth();
    int height = mDisplacementMap.getHeight();

    //initialize the normal map
    mNormalMap.init(width, height, mDisplacementMap.getPixelFormat());

    //define max pixel value
    const int MAX_PX_VALUE = 255;

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
                left = (float)(mDisplacementMap.getPixelValue(j - 1, i).getR() - mDisplacementMap.getPixelValue(j, i).getR() + MAX_PX_VALUE) / (float)(2 * MAX_PX_VALUE);
                right = (float)(mDisplacementMap.getPixelValue(j, i).getR() - mDisplacementMap.getPixelValue(0, i).getR() + MAX_PX_VALUE) / (float)(2 * MAX_PX_VALUE);
                averageHorizontal = (left + right) / 2.0f;
            }
            else if (j == 0)
            {
                left = (float)(mDisplacementMap.getPixelValue(width - 1, i).getR() - mDisplacementMap.getPixelValue(j + 1, i).getR() + MAX_PX_VALUE) / (float)(2 * MAX_PX_VALUE);
                right = (float)(mDisplacementMap.getPixelValue(j, i).getR() - mDisplacementMap.getPixelValue(j + 1, i).getR() + MAX_PX_VALUE) / (float)(2 * MAX_PX_VALUE);
                averageHorizontal = (left + right) / 2.0f;
            }
            else
            {
                left = averageHorizontal = (float)(mDisplacementMap.getPixelValue(j - 1, i).getR() - mDisplacementMap.getPixelValue(j, i).getR() + MAX_PX_VALUE) / (float)(2 * MAX_PX_VALUE);
                right = averageHorizontal = (float)(mDisplacementMap.getPixelValue(j, i).getR() - mDisplacementMap.getPixelValue(j + 1, i).getR() + MAX_PX_VALUE) / (float)(2 * MAX_PX_VALUE);
                averageHorizontal = (left + right) / 2.0f;
            }

            //evaluate up-down curves
            if (i == height - 1)
            {
                above = (float)(mDisplacementMap.getPixelValue(j, i).getR() - mDisplacementMap.getPixelValue(j, i - 1).getR() + MAX_PX_VALUE) / (float)(2 * MAX_PX_VALUE);
                below = (float)(mDisplacementMap.getPixelValue(j, 0).getR() - mDisplacementMap.getPixelValue(j, i).getR() + MAX_PX_VALUE) / (float)(2 * MAX_PX_VALUE);
                averageVertical = (above + below) / 2.0f;
            }
            else if (i == 0)
            {
                above = (float)(mDisplacementMap.getPixelValue(j, i).getR() - mDisplacementMap.getPixelValue(j, height - 1).getR() + MAX_PX_VALUE) / (float)(2 * MAX_PX_VALUE);
                below = (float)(mDisplacementMap.getPixelValue(j, i + 1).getR() - mDisplacementMap.getPixelValue(j, i).getR() + MAX_PX_VALUE) / (float)(2 * MAX_PX_VALUE);
                averageVertical = (above + below) / 2.0f;
            }
            else
            {
                above = (float)(mDisplacementMap.getPixelValue(j, i).getR() - mDisplacementMap.getPixelValue(j, i - 1).getR() + MAX_PX_VALUE) / (float)(2 * MAX_PX_VALUE);
                below = (float)(mDisplacementMap.getPixelValue(j, i + 1).getR() - mDisplacementMap.getPixelValue(j, i).getR() + MAX_PX_VALUE) / (float)(2 * MAX_PX_VALUE);
                averageVertical = (above + below) / 2.0f;
            }

            r = averageHorizontal * MAX_PX_VALUE;
            g = averageVertical * MAX_PX_VALUE;
            float horizontalDiff = std::abs(0.5f - averageHorizontal);
            float verticalDiff = std::abs(0.5f - averageVertical);
            float diffFactor = 1.0f - ((horizontalDiff + verticalDiff));
            b = diffFactor * MAX_PX_VALUE;
            mNormalMap.setPixelValue(Color(r, g, b), j, i);
        }
    }
}

Image& TextureSet::getDiffuseMap()
{
	return mDiffuseMap;
}

Image& TextureSet::getDisplacementMap()
{
    return mDisplacementMap;
}

Image& TextureSet::getNormalMap()
{
	return mNormalMap;
}

bool TextureSet::loadDiffuseMapFromFile(std::string filename)
{
	return mDiffuseMap.readFromFileAsPPM(filename);
}

bool TextureSet::loadDisplacementMapFromFile(std::string filename)
{
    return mDisplacementMap.readFromFileAsPPM(filename);
}

bool TextureSet::loadNormalMapFromFile(std::string filename)
{
    return mNormalMap.readFromFileAsPPM(filename);
}