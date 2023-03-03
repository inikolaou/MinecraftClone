#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <GL/glew.h>
#include "stb_image.h"
#include <vector>
#include <string>

using namespace std;

class TextureManager
{
public:
	TextureManager(int textureWidth, int textureHeight, int maxTextures);

	void generateMipmaps();

	void addTexture(const std::string& texture);

	GLuint textureArray;

	vector<string> textures;
private:
	int textureWidth;
	int textureHeight;
	int maxTextures;
};

#endif TEXTUREMANAGER_H