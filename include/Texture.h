#include <iostream>
#include "glad.h"

#pragma once

class Texture
{
private:
    u_char*  mTextureRawData;
    uint32_t mTextureId;
    int mTextureWidth;
    int mTextureHeight;
    int mTextureColorChannels;
    uint32_t mTargetType;
public:
    Texture(const std::string& texturePath, uint32_t targetType=GL_TEXTURE_2D);
    ~Texture();

    void bind();
    void unbind();
    void setDefaultTextureParams();
    void loadToGpu();
    void generateMipmap();
};