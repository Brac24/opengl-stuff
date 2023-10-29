#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const std::string& texturePath, uint32_t targetType):
mTargetType(targetType)
{
    // load the image data of the container crate
    mTextureRawData = stbi_load(texturePath.c_str(), &mTextureWidth, &mTextureHeight, &mTextureColorChannels, 0);
    glGenTextures(1, &mTextureId);
}

Texture::~Texture(){}

void Texture::bind()
{
    glBindTexture(mTargetType, mTextureId); // binds a texture id to a texture target type such as GL_TEXTURE_2D
}

void Texture::unbind()
{
    glBindTexture(mTargetType, 0);
}

void Texture::loadToGpu()
{
    glTexImage2D(mTargetType, 0, GL_RGB, mTextureWidth, mTextureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, mTextureRawData);
    stbi_image_free(mTextureRawData);
}

void Texture::setDefaultTextureParams()
{
    // These default parameter settings were set based on the following article
    // https://learnopengl.com/Getting-started/Textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::generateMipmap()
{
    glGenerateMipmap(mTargetType);
}