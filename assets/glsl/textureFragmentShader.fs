#version 430 core
out vec4 FragColor; 
in vec4 ourColor; 
in vec2 TexCoordinate;

uniform sampler2D ourTexture; // This will contain the currently bound texture.

void main()
{
  FragColor = texture(ourTexture, TexCoordinate)*ourColor; // This samples the color of the texture in ourTexture at its texture coordinate TexCoordinate
}