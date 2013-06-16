#version 330 core

in vec2 texCoord;
out vec3 color;

uniform sampler2D sampler;

void main()
{
  color = texture( sampler, texCoord ).rgb;
}
