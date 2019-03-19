#version 330
layout(location = 0) in vec3 posModel;
layout(location = 1) in vec2 vertexUV;

out vec2 UV;
out float viewerDistance;

uniform mat4 projMx;
uniform mat4 viewMx;
uniform mat4 modelMx;

void main()
{
  mat4 VMsprite = viewMx * modelMx;
  VMsprite[0] = vec4(length(modelMx[0]), 0.0, 0.0, 0.0);
  VMsprite[1] = vec4(0.0, length(modelMx[1]), 0.0, 0.0);
  VMsprite[2] = vec4(0.0, 0.0, length(modelMx[2]), 0.0);
  VMsprite *= inverse(modelMx);
  gl_Position = projMx * VMsprite * vec4(posModel, 1.0);
  UV = vertexUV;
  viewerDistance = length(viewMx * modelMx * vec4(0.0, 0.0, 0.0, 1.0));
}