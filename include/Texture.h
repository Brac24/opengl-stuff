#include <iostream>
#include "glad.h"

#pragma once

/**
 * @brief Texture class meant to hold information and peform actions on a texture.
 * The current implementation is mainly to be used with images as textures.
 * Pass in the path to the image you want to use as a texture and this will convert the
 * image into char array of image data. Call bind on the texture to have OpenGL create
 * storage for the texture. Call loadToGpu to send the raw image data to the GPU where it will
 * get stored in a buffer that was created when we call the bind() function on the texture.
 * 
 */
class Texture
{
private:
    u_char*  mTextureRawData;
    unsigned int mTextureId;
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

    /**
     * @brief Generates a Mipmap for this texture.
     * Mipmaps is a set of different size textures that can be used in place of the originally sized texture.
     * This allows for more efficiency when a texture only takes up a very small part of the window and a smaller
     * texture with less detail can be used in place. This scenario would mainly be used for 3D perspective projection
     * style projects and might not be necessary for 2D orthographic projection scenarios. This function calls an
     * OpenGL API function that has OpenGL create these smaller sized textures from the original texture we have loaded and bound.
     * 
     */
    void generateMipmap();
};