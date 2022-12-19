#version 430 core
layout (location = 0) in vec3 aPos; 
layout (location = 1) in vec4 aColor;

uniform vec3 attenuate;

out vec4 ourColor;

void main()
{
  gl_Position = vec4(aPos, 1.0);

  // attenuating the color and and tranparency at vertex positions over time
  ourColor = vec4(aColor.rgb * attenuate, attenuate.r*aColor.a);
}