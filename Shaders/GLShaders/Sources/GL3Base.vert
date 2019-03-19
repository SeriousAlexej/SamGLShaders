#version 330
layout(location = 0) in vec3 posModel;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 normalModel;

out vec2 UV;
out vec3 normal;

uniform mat4 projMx;
uniform mat4 viewMx;
uniform mat4 modelMx;

void main()
{
  gl_Position = projMx * viewMx * vec4(posModel, 1.0);
  UV = vertexUV;
  normal = (modelMx * vec4(normalModel, 0.0)).xyz;
}