#version 330
layout(location = 0) in vec3 posModel;
layout(location = 2) in vec3 normalModel;

out vec3 normal;
out vec3 eyePos;
out vec3 fragPos;

uniform mat4 projMx;
uniform mat4 viewMx;
uniform mat4 modelMx;

void main()
{
  gl_Position = projMx * viewMx * vec4(posModel, 1.0);
  fragPos = vec3(posModel);
  normal = normalModel;
  eyePos = vec3(inverse(viewMx) * vec4(0.0, 0.0, 0.0, 1.0));
}