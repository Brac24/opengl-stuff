#version 430 core

// This vertex shader was created for use with textures.
// This shader runs for each vertex.
// Taking an example using a quad (i.e. a square object) there are 4 vertices for quad.
// Therefore, this shader will run 4 times, once for each vertex in the quad.
// When using this shader we mainly only care about textures so the only important attirbutes here
// are aPos and aTexCoordinate.
// aPos will place the vertices in space in the window. aTexCoordinate will relate a coordinate on the
// texture image with the corresponding aPos vertex.
// For example, when this shader runs for the bottom left vertex of the quad we would likely have an
// aTexCoordinate of (0,0) because we want the bottom left of our quad to be connected with the bottom
// left of the image we want to place on our quad.
// Remember that texture coordinates start at (0,0) as the bottom left of the image and (1,1) being
// the top right of the image while OpenGL window origin is the center of the window being (0,0).

// Essentially we are saying hey for this vertex aPos of our square, make sure it grabs/samples the color
// of our image/texture at the texture coordinate aTexCoordinate.
// **Note that the actual texture color sampling occurs in the fragment shader though not here.
//   This is just a way to be able to pass to our fragment shader where to sample from for each vertex.

layout (location = 0) in vec3 aPos; 
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoordinate;

uniform vec3 attenuate;

out vec4 ourColor;
out vec2 TexCoordinate;

void main()
{
  gl_Position = vec4(aPos, 1.0);

  // attenuating the color and and tranparency at vertex positions over time
  ourColor = vec4(aColor.rgb * attenuate, attenuate.r*aColor.a);
  TexCoordinate = aTexCoordinate;
}