#include "TextureManager.h"

TextureManager::TextureManager(int textureWidth, int textureHeight, int maxTextures) : 
	textureWidth{ textureWidth }, textureHeight{ textureHeight }, maxTextures{ maxTextures } {

	glGenTextures(1, &textureArray);
	glBindTexture(GL_TEXTURE_2D_ARRAY, textureArray);

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage3D(
		GL_TEXTURE_2D_ARRAY, 0, GL_RGBA,
		textureWidth, textureHeight, maxTextures,
		0, GL_RGBA, GL_UNSIGNED_BYTE, NULL
	);


}

void TextureManager::generateMipmaps() {
	glBindTexture(GL_TEXTURE_2D_ARRAY, textureArray);
	glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
}

void TextureManager::addTexture(const std::string& texture) {
	auto it = find(textures.begin(),
		textures.end(), texture);
	if (it != textures.end())
		return;
	textures.push_back(texture);

	int width, height, channels;

	unsigned char* data = stbi_load(("../textures/" + texture + ".png").c_str(), &width, &height, &channels, STBI_rgb_alpha);
	
	glBindTexture(GL_TEXTURE_2D_ARRAY, textureArray);
	glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0,
		0, 0, textures.size() - 1,
		textureWidth, textureHeight, 1,
		GL_RGBA, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);
}