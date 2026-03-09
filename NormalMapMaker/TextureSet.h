#ifndef TEXTURESET_H
#define TEXTURESET_H

#include <string>
#include "Image.h"

class TextureSet
{
	public:
		TextureSet();	
		~TextureSet();

		void cleanup();

		//accessors
		Image& getDiffuseMap();
		Image& getDisplacementMap();
		Image& getNormalMap();
		
		//converters
		void generateNormalFromDisplacement();

		//loading
		bool loadDiffuseMapFromFile(std::string filename); //take filename WITHOUT the extension
		bool loadDisplacementMapFromFile(std::string filename);
		bool loadNormalMapFromFile(std::string filename);

	private:
		void generateNormalFromDisplacement_PPM255();

	private:
		//maps
		Image mDiffuseMap;
		Image mDisplacementMap;
		Image mNormalMap;
};

#endif