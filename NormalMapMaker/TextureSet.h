#ifndef TEXTURESET_H
#define TEXTURESET_H

#include "Image.h"

class TextureSet
{
	public:
		TextureSet();
		~TextureSet();

	private:
		Image diffuseMap;
		Image normalMap;
};

#endif