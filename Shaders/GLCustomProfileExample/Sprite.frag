#version 330
out vec4 color;

in vec2 UV;

uniform sampler2D diffuseTexture;

void main()
{
  color = texture(diffuseTexture, UV).rgba;
}